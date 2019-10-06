#ifndef _SERVER_RESPONSE_H_
#define _SERVER_RESPONSE_H_

#include "response.h"

class Server_response final : public Response {
public:
	Server_response(Code response_code);
	Code get_response_code() const;
	void set_error();
	void set_success();

private:
	Code response_code_;
};

#endif // !_SERVER_RESPONSE_H_
