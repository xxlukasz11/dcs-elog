#include "attachment_handler_tx.h"
#include "json_stringifier.h"
#include "json/json.h"
#include "utils.h"
#include "custom_exceptions.h"
#include "administrator.h"
#include "website_response.h"
#include "base64.h"
#include <utility>
#include <string>
#include <fstream>

namespace {

const std::string PAYLOAD_LABEL_NAME = "Payload";
const std::string PAYLOAD_PLACEHOLDER = "$:#;&";
constexpr size_t TX_BUFFER_SIZE = 99999;
static_assert(TX_BUFFER_SIZE % 3 == 0, "TX buffer size must match Base64 encoding");

enum class Attachment_table {
	ATTACHMENT_ID = 0,
	EVENT_ID,
	ATTACHMENT_NAME,
	ATTACHMENT_TYPE,
	FILE_NAME
};

int column_index(Attachment_table column_identifier) {
	return static_cast<int>(column_identifier);
}

json::Object_ptr get_object(json::Array_ptr array, size_t index) {
	return std::static_pointer_cast<json::Object>(array->get(index));
}

Result_set filter_by_event_id(const Result_set& attachments, const std::string& event_id) {
	return attachments.filter([&event_id](const auto& row) {
		return row[column_index(Attachment_table::EVENT_ID)] == event_id;
	});
}

json::Array_ptr create_json_attachments_array(const Result_set& attachments) {
	auto attachmemts_json = Json_stringifier::to_json_array(attachments);
	auto size = attachmemts_json->size();
	for (int i = 0; i < size; ++i) {
		auto attachment_object = get_object(attachmemts_json, i);
		attachment_object->add_field(PAYLOAD_LABEL_NAME, json::Json::string(PAYLOAD_PLACEHOLDER));
	}
	return attachmemts_json;
}

std::string create_attachment_path(const std::string& file_name) {
	return Administrator::instance().params().get_attachment_storage_path() + file_name;
}

size_t fill_buffer(std::istream& stream, std::vector<unsigned char>& buffer, size_t bytes_to_read) {
	buffer.resize(bytes_to_read);
	stream.read(reinterpret_cast<char*>(buffer.data()), bytes_to_read);
	if (!stream) {
		auto read_count = stream.gcount();
		stream.clear();
		buffer.resize(read_count);
		return read_count;
	}
	return bytes_to_read;
}

} // namespace

Attachment_handler_tx::Attachment_handler_tx(Socket socket) : socket_(socket) {
}

void Attachment_handler_tx::inject_payload_and_send() {
	std::string string_to_send = create_response_string();
	if (sequenced_attachments_.size() == 0) {
		Logger::create().context(socket_).level(Log_level::ALL).info("Sending response without attachments");
		send_plain(string_to_send);
	}
	else {
		Logger::create().context(socket_).level(Log_level::ALL).info("Sending response with attachments");
		send_with_attachment_payload(std::move(string_to_send));
	}
	Logger::create().context(socket_).level(Log_level::ALL).info("Response successfully sent");
}

void Attachment_handler_tx::set_events(Result_set&& events) {
	events_ = std::move(events);
}

void Attachment_handler_tx::set_attachments(Result_set&& attachments) {
	attachments_ = std::move(attachments);
}

void Attachment_handler_tx::set_on_success_message(const std::string message) {
	on_success_message_ = message;
}

void Attachment_handler_tx::set_on_failure_message(const std::string message) {
	on_failure_message_ = message;
}

std::string Attachment_handler_tx::create_events_string() {
	auto events_json = Json_stringifier::to_json_array(events_);
	auto no_of_events = events_.data_size();
	for (int i = 0; i < no_of_events; ++i) {
		std::string event_id = events_.get_field(i, 0);

		Result_set filtered_attachments = filter_by_event_id(attachments_, event_id);
		auto attachments_json = create_json_attachments_array(filtered_attachments);
		move_attachments_file_names(std::move(filtered_attachments));

		json::Object_ptr event_object = get_object(events_json, i);
		event_object->add_field("Attachments", attachments_json);
	}
	return events_json->to_string();
}

std::string Attachment_handler_tx::create_response_string() {
	std::string stringified_events = create_events_string();
	Website_response response;
	response.set_success(on_success_message_);
	response.set_data(std::move(stringified_events));
	return Json_stringifier::stringify(std::move(response));
}

void Attachment_handler_tx::assert_valid_attachments_size(int size) {
	if (sequenced_attachments_.size() != size) {
		throw Attachment_error("Error while sending attachments. Expected amount: ",
			size, " but was: ", sequenced_attachments_.size());
	}
}

void Attachment_handler_tx::send_plain(const std::string& string) {
	socket_.send_string(string);
}

void Attachment_handler_tx::send_with_attachment_payload(std::string&& string) {
	std::vector<std::string> chunks = utils::string_to_vector(string, PAYLOAD_PLACEHOLDER);
	auto no_of_chunks = chunks.size();
	assert_valid_attachments_size(no_of_chunks - 1);

	for (size_t i = 0; i < no_of_chunks; ++i) {
		send_plain(chunks[i]);
		if (i != no_of_chunks - 1) {
			send_attachment(sequenced_attachments_[i]);
		}
	}
}

void Attachment_handler_tx::send_attachment(const std::string& file_name) {
	std::string attachment_path = create_attachment_path(file_name);
	Logger::create().context(socket_).level(Log_level::ALL).info("Sending " + attachment_path);
	std::ifstream file(attachment_path, std::ios::binary);
	
	Base64 encoder;
	std::vector<unsigned char> buffer;
	while (true) {
		size_t bytes_to_sent = fill_buffer(file, buffer, TX_BUFFER_SIZE);
		if (bytes_to_sent == 0) {
			break;
		}
		auto encoded_buffer = encoder.encode(buffer);
		socket_.send_buffer(encoded_buffer);
	}
	Logger::create().context(socket_).level(Log_level::ALL).info("Attachment successfully sent");
}

void Attachment_handler_tx::move_attachments_file_names(Result_set&& attachments) {
	auto no_of_attachments = attachments.data_size();
	for (size_t i = 0; i < no_of_attachments; ++i) {
		auto& file_name = attachments.get_row(i)[column_index(Attachment_table::FILE_NAME)];
		sequenced_attachments_.push_back(std::move(file_name));
	}
}
