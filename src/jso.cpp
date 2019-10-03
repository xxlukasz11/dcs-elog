#include "jso.h"

namespace json {

Json_string_ptr json::Json::string(const std::string& value) {
	return Json_string::create(value);
}

Json_array_ptr Json::array() {
	return Json_array::create();
}

Json_object_ptr Json::object() {
	return Json_object::create();
}

}
