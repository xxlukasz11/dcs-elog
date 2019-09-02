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

Unknown_message_format::Unknown_message_format() {
	message_ = "Recieved message has unknown format";
}

const std::string& Unknown_message_format::what() const noexcept {
	return message_;
}
