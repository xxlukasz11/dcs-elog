#include "json_object.h"

namespace json {

std::string Json_object::to_string() const {
	return CURLY_BRACE_OPEN + fields_to_string() + CURLY_BRACE_CLOSE;
}

void Json_object::add_field(const std::string& name, const Json_value_ptr& value) {
	fields_.emplace_back(name, value);
}

Json_object_ptr Json_object::create() {
	return std::make_shared<Json_object>();
}

std::string Json_object::fields_to_string() const {
	std::string result;
	size_t no_of_fields = fields_.size();
	for (size_t i = 0; i < no_of_fields; ++i) {
		result += fields_[i].to_string();
		if (i != no_of_fields - 1) {
			result += COMMA;
		}
	}
	return result;
}

}
