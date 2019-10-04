#include <utility>
#include "jso.h"
#include "json_stringifier.h"

using namespace json;

namespace {

Json_object_ptr create_event_object(size_t row_index, const Result_set& result_set) {
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

Json_object_ptr create_event_object(size_t row_index, Result_set&& result_set) {
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

} // namespace

std::string Json_stringifier::stringify(const Result_set& result_set) {
	auto array = Json::array();
	size_t length = result_set.data_size();
	for (size_t i = 0; i < length; ++i) {
		auto event_object = create_event_object(i, result_set);
		array->push(event_object);
	}
	return array->to_string();
}

std::string Json_stringifier::stringify(Result_set&& result_set) {
	auto array = Json::array();
	size_t length = result_set.data_size();
	for (size_t i = 0; i < length; ++i) {
		auto event_object = create_event_object(i, std::move(result_set));
		array->push(event_object);
	}
	return array->to_string();
}
