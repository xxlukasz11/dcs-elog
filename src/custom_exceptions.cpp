#include "custom_exceptions.h"

const char* Timeout_error::what() const noexcept {
	return "Connection timed out";
}

const char* Client_disconnected_error::what() const noexcept {
	return "Client disconnected";
}

const char* Send_error::what() const noexcept {
	return "Error while sending data to client";
}

const char* Unknown_message_format::what() const noexcept {
	return "Recieved message has unknown format";
}