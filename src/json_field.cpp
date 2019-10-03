#include "json_field.h"

namespace json {

std::string Json_field::to_string() const {
	return DOUBLE_QUOTES + label_ + DOUBLE_QUOTES + COLON + value_->to_string();
}

Json_field::Json_field(const std::string& label, const Json_value_ptr& value) : label_(label), value_(value) {
}

} // namespace json