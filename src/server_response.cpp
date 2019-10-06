#include "server_response.h"

Server_response::Server_response(Code response_code) : response_code_(response_code) {
}

Response::Code Server_response::get_response_code() const {
	return response_code_;
}

void Server_response::set_error() {
	response_code_ = Response::Code::FAILURE;
}

void Server_response::set_success() {
	response_code_ = Response::Code::SUCCESS;
}
