#include <utility>
#include "json/json.h"
#include "json_stringifier.h"

using namespace json;

namespace {

Object_ptr create_event_object(size_t row_index, const Result_set& result_set) {
	auto event_object = Json::object();
	size_t row_length = result_set.header_length();
	auto& header = result_set.get_header();
	for (int field_index = 0; field_index < row_length; ++field_index) {
		auto& label = header[field_index];
		auto value = Json::string(result_set.get_row(row_index)[field_index]);
		event_object->add_field(label, value);
	}
	return event_object;
}

Object_ptr create_event_object(size_t row_index, Result_set&& result_set) {
	auto event_object = Json::object();
	size_t row_length = result_set.header_length();
	Result_set::header_type& header = result_set.get_header();

	for (int field_index = 0; field_index < row_length; ++field_index) {
		auto& label = header[field_index];
		auto& string_value = result_set.get_row(row_index)[field_index];
		auto value = Json::string(std::move(string_value));
		event_object->add_field(std::move(label), value);
	}
	return event_object;
}

Object_ptr create_response_object(Response::Code code) {
	int integer_code = static_cast<int>(code);
	auto response_object = Json::object();
	response_object->add_field("error_code", Json::number(integer_code));
	return response_object;
}

template<typename Message, typename Data>
Object_ptr create_response_object(Response::Code code, Message&& message, Data&& data) {
	auto response_object = create_response_object(code);
	response_object->add_field("message", Json::string(std::forward<Message>(message)));
	if (data.empty()) {
		response_object->add_field("data", Json::string());
	}
	else {
		response_object->add_field("data", Json::json_string(std::forward<Data>(data)));
	}
	return response_object;
}

} // namespace

std::string Json_stringifier::stringify(const Result_set& result_set) {
	auto array = to_json_array(result_set);
	return array->to_string();
}

std::string Json_stringifier::stringify(Result_set&& result_set) {
	auto array = to_json_array(std::move(result_set));
	return array->to_string();
}

Array_ptr Json_stringifier::to_json_array(const Result_set& result_set) {
	auto array = Json::array();
	size_t length = result_set.data_size();
	for (size_t i = 0; i < length; ++i) {
		auto event_object = create_event_object(i, result_set);
		array->push(event_object);
	}
	return array;
}

Array_ptr Json_stringifier::to_json_array(Result_set&& result_set) {
	auto array = Json::array();
	size_t length = result_set.data_size();
	for (size_t i = 0; i < length; ++i) {
		auto event_object = create_event_object(i, std::move(result_set));
		array->push(event_object);
	}
	return array;
}

std::string Json_stringifier::stringify(const Website_response& response) {
	auto response_object = create_response_object(
		response.get_response_code(),
		response.get_message(),
		response.get_data()
	);
	return response_object->to_string();
}

std::string Json_stringifier::stringify(Website_response&& response) {
	auto response_object = create_response_object(
		response.get_response_code(),
		std::move(response.get_message()),
		std::move(response.get_data())
	);
	return response_object->to_string();
}

std::string Json_stringifier::stringify(const Server_response& response) {
	auto response_object = create_response_object(response.get_response_code());
	return response_object->to_string();
}
