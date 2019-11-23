#include "custom_exceptions.h"

Timeout_error::Timeout_error() {
	message_ = "Connection timed out";
}

const std::string& Timeout_error::what() const noexcept {
	return message_;
}

Client_disconnected_error::Client_disconnected_error() {
	message_ = "Client disconnected";
}

const std::string& Client_disconnected_error::what() const noexcept {
	return message_;
}

Send_error::Send_error() {
	message_ = "Error while sending data to client";
}

const std::string& Send_error::what() const noexcept {
	return message_;
}

Unknown_message_format::Unknown_message_format(const std::string& message_name) {
	message_ = message_name + " has unknown format";
}

Unknown_message_format::Unknown_message_format(const std::string& message_name, const std::string& error_description) {
	message_ = message_name + " has unknown format: " + error_description;
}

const std::string& Unknown_message_format::what() const noexcept {
	return message_;
}

Database_error::Database_error(const std::string& description, int error_code, const char* error_msg) :
	message_(description + "; code: " + std::to_string(error_code) + "; msg: " + error_msg) {
}

Database_error::Database_error(const std::string& description) :
	message_(description) {
}

int Database_error::get_error_code() const {
	return error_code_;
}

const std::string& Database_error::what() const noexcept {
	return message_;
}

