#include <utility>
#include "json/json.h"

namespace json {

Array_ptr Json::array() {
	return Array::create();
}

Object_ptr Json::object() {
	return Object::create();
}

String_ptr Json::string(const std::string& value) {
	return String::create(value);
}

String_ptr Json::string(std::string&& value) {
	return String::create(std::move(value));
}

Json_string_ptr Json::json_string(const std::string& value) {
	return Json_string::create(value);
}

Json_string_ptr Json::json_string(std::string&& value) {
	return Json_string::create(std::move(value));
}

}
