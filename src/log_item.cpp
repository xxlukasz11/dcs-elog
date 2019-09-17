#include "log_item.h"

Log_item::Log_item() : time_(std::time(nullptr)) {
}

bool Log_item::has_context() const {
	return context_is_set_;
}

bool Log_item::has_location() const {
	if (location_.empty()) {
		return false;
	}
	return true;
}

time_t Log_item::get_time() const {
	return time_;
}

int Log_item::get_context() const {
	return context_;
}

const std::string& Log_item::get_location() const {
	return location_;
}

const std::string& Log_item::get_message() const {
	return message_;
}

Log_item::Type Log_item::get_type() const {
	return type_;
}

void Log_item::set_context(int context) {
	context_ = context;
	context_is_set_ = true;
}

void Log_item::set_location(const std::string& location) {
	location_ = location;
}

void Log_item::set_message(Log_item::Type type, const std::string& message) {
	type_ = type;
	message_ = message;
}
