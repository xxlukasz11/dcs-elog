#ifndef _WEBSITE_RESPONSE_H_
#define _WEBSITE_RESPONSE_H_

#include "response.h"
#include "result_set.h"
#include <string>

/*
Response sent to the website
*/
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
	void set_data(const Result_set& data);
	void set_data(Result_set&& data);

private:
	Code response_code_{ Code::FAILURE };
	std::string message_;
	std::string data_;
};

#endif // !_WEBSITE_RESPONSE_H_
