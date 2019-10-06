#ifndef _WEBSITE_RESPONSE_H_
#define _WEBSITE_RESPONSE_H_

#include "response.h"
#include <string>

class Website_response final : public Response {
public:
	Code get_response_code() const;
	const std::string& get_message() const;
	const std::string& get_data() const;
	std::string& get_message();
	std::string& get_data();
	void set_failure(const std::string& message);
	void set_success(const std::string& message);
	void set_data(const std::string& data);
	void set_data(std::string&& data);

private:
	Code response_code_{ Code::FAILURE };
	std::string message_;
	std::string data_;
};

#endif // !_WEBSITE_RESPONSE_H_
