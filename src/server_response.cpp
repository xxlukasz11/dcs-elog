#include "server_response.h"

Response::Code Server_response::get_response_code() const {
	return response_code_;
}

void Server_response::set_failure() {
	response_code_ = Response::Code::FAILURE;
}

void Server_response::set_success() {
	response_code_ = Response::Code::SUCCESS;
}
