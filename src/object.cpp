#include "json/object.h"

namespace json {

std::string Object::to_string() const {
	return CURLY_BRACE_OPEN + fields_to_string() + CURLY_BRACE_CLOSE;
}

void Object::add_field(const std::string& name, const Value_ptr& value) {
	fields_.emplace_back(name, value);
}

Object_ptr Object::create() {
	return std::make_shared<Object>();
}

std::string Object::fields_to_string() const {
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
