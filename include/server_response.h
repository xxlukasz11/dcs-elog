#ifndef _SERVER_RESPONSE_H_
#define _SERVER_RESPONSE_H_

#include "response.h"

class Server_response final : public Response {
public:
	Code get_response_code() const;
	void set_failure();
	void set_success();

private:
	Code response_code_{ Code::FAILURE };
};

#endif // !_SERVER_RESPONSE_H_
