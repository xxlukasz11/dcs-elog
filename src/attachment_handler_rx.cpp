#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include "attachment_handler_rx.h"
#include "logger.h"
#include "base64.h"
#include "file_name_parser.h"
#include "custom_exceptions.h"
#include "administrator.h"
#include "utils.h"

constexpr int RX_BUFFER_SIZE = 100000;
static const std::string FILE_NAME_FORMAT = "%Y_%m_%d_%H_%M_%S";
static const std::string UNDERSCORE = "_";

std::atomic<unsigned long> Attachment_handler_rx::attachment_control_index_{ 0 };

Attachment_handler_rx::Attachment_handler_rx(Socket socket) : socket_(socket) {
}

Attachment_handler_rx::~Attachment_handler_rx() {
	if (!commited_) {
		delete_attachments_from_disc();
	}
}

void Attachment_handler_rx::handle_attachments(const Attachment_info_array& attachments_info) {
	attachment_array_.clear();
	for (const auto& att_info : attachments_info) {
		receive_and_save_attachment(att_info);
	}
}

void Attachment_handler_rx::receive_and_save_attachment(const Attachment_info& attachment_info) {
	auto file_size = receive_attachment_size();
	log_incoming_attachment_info(attachment_info, file_size);

	Base64 base64;
	int bytes_received = 0;
	int binary_file_size = 0;
	std::ofstream file = create_unique_file(attachment_info);
	std::vector<unsigned char> buffer(RX_BUFFER_SIZE);
	while (bytes_received < file_size) {
		int current_buffer_size = std::min(RX_BUFFER_SIZE, file_size - bytes_received);
		socket_.fill_buffer(buffer, current_buffer_size);
		bytes_received += current_buffer_size;
		auto decoded = base64.decode(buffer, current_buffer_size);
		binary_file_size += decoded.size();
		file.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
	}
	file.close();
	Logger::create().context(socket_).level(Log_level::INFO).info("File " + attachment_info.get_name() +
		" successfully received. Saved file size: " + std::to_string(binary_file_size) + " bytes");
}

void Attachment_handler_rx::delete_attachments_from_disc() {
	std::string storage_path = Administrator::instance().params().get_attachment_storage_path();
	for (const auto& attachment : attachment_array_) {
		std::string file_path = storage_path + attachment.get_file_name();
		auto status = remove(file_path.c_str());
		if (status != 0) {
			Logger::create().context(socket_).level(Log_level::WARNING).warning("Failed to remove file: " +
				attachment.get_file_name() + " aka " + attachment.get_name());
		}
	}
}

void Attachment_handler_rx::commit() {
	commited_ = true;
}

const Attachment_database_info_array& Attachment_handler_rx::get_attachment_array() const {
	return attachment_array_;
}

std::ofstream Attachment_handler_rx::create_unique_file(const Attachment_info& attachment_info) {
	std::string unique_file_name = create_unique_file_name(attachment_info.get_name());
	attachment_array_.emplace_back(attachment_info.get_name(), attachment_info.get_type(), unique_file_name);

	std::string path = create_attachment_path(unique_file_name);
	Logger::create().context(socket_).level(Log_level::ALL).info("Saving " + attachment_info.get_name() + " in " + path);
	return std::ofstream(path, std::ios::binary);
}

std::string Attachment_handler_rx::generate_file_name_discriminator() {
	return std::to_string(attachment_control_index_++);
}

std::string Attachment_handler_rx::create_attachment_path(const std::string& file_name) {
	return Administrator::instance().params().get_attachment_storage_path() + file_name;
}

std::string Attachment_handler_rx::create_unique_file_name(const std::string& file_name) {
	File_name_parser parser;
	if (!parser.parse(file_name)) {
		throw Attachment_error("Invalid file name: ", file_name);
	}
	time_t now = std::time(0);
	return utils::create_date_time_string(now, FILE_NAME_FORMAT)
		+ UNDERSCORE
		+ generate_file_name_discriminator()
		+ parser.get_extension();
}

int Attachment_handler_rx::receive_attachment_size() {
	int file_size = socket_.receive<int>();
	return file_size;
}

void Attachment_handler_rx::log_incoming_attachment_info(const Attachment_info& attachment_info, int file_size) {
	Logger::create().context(socket_).level(Log_level::ALL).info("Receiving " + attachment_info.get_name() +
		" of type: " + attachment_info.get_type() + " - size: " + std::to_string(file_size) + " bytes");
}
