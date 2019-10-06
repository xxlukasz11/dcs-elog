#include <utility>
#include "website_response.h"

Response::Code Website_response::get_response_code() const {
	return response_code_;
}

const std::string& Website_response::get_message() const {
	return message_;
}

const std::string& Website_response::get_data() const {
	return data_;
}

std::string& Website_response::get_message() {
	return message_;
}

std::string& Website_response::get_data() {
	return data_;
}

void Website_response::set_error(const std::string& message) {
	response_code_ = Response::Code::FAILURE;
	message_ = message;
}

void Website_response::set_success(const std::string& message) {
	response_code_ = Response::Code::SUCCESS;
	message_ = message;
}

void Website_response::set_data(const std::string& data) {
	data_ = data;
}

void Website_response::set_data(std::string&& data) {
	data_ = std::move(data);
}
