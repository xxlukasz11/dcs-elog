#include "json/json.h"

namespace json {

String_ptr json::Json::string(const std::string& value) {
	return String::create(value);
}

Array_ptr Json::array() {
	return Array::create();
}

Object_ptr Json::object() {
	return Object::create();
}

}
