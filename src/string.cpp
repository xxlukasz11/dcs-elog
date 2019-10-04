#include <utility>
#include "json/string.h"

namespace json {

std::string String::to_string() const {
	return DOUBLE_QUOTES + value_ + DOUBLE_QUOTES;
}

String::String(const std::string& value) : value_(value) {
}

String::String(std::string&& value) : value_(std::move(value)) {
}

String_ptr String::create(const std::string& value) {
	return std::make_shared<String>(value);
}

String_ptr String::create(std::string&& value) {
	return std::make_shared<String>(std::move(value));
}

}
