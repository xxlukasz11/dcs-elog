#include "json/field.h"

namespace json {

std::string Field::to_string() const {
	return DOUBLE_QUOTES + label_ + DOUBLE_QUOTES + COLON + value_->to_string();
}

Field::Field(const std::string& label, const Value_ptr& value) : label_(label), value_(value) {
}

} // namespace json