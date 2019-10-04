#include <utility>
#include "json_string.h"

namespace json {

std::string Json_string::to_string() const {
	return DOUBLE_QUOTES + value_ + DOUBLE_QUOTES;
}

Json_string::Json_string(const std::string& value) : value_(value) {
}

Json_string::Json_string(std::string&& value) : value_(std::move(value)) {
}

Json_string_ptr Json_string::create(const std::string& value) {
	return std::make_shared<Json_string>(value);
}

Json_string_ptr Json_string::create(std::string&& value) {
	return std::make_shared<Json_string>(std::move(value));
}

}
