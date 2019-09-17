#include "log_item.h"

Log_item::Log_item() : time_(std::time(nullptr)) {
}

bool Log_item::has_context() const {
	if (context_) {
		return true;
	}
	return false;
}

bool Log_item::has_location() const {
	if (location_) {
		return true;
	}
	return false;
}

time_t Log_item::get_time() const {
	return time_;
}

int Log_item::get_context() const {
	return context_.value_or(0);
}

const std::string& Log_item::get_location() const {
	return location_.value();
}

const std::string& Log_item::get_message() const {
	return message_;
}

Log_item::Type Log_item::get_type() const {
	return type_;
}

void Log_item::set_context(int context) {
	context_.emplace(context);
}

void Log_item::set_location(const std::string& location) {
	location_.emplace(location);
}

void Log_item::set_message(Log_item::Type type, const std::string& message) {
	type_ = type;
	message_ = message;
}
