#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <memory>
#include "message.h"

namespace utils{
	std::string get_date_time_str();
	std::string concatenate_string_array(const std::vector<std::string>& array);
	std::vector<std::string> string_to_vector(std::string string);

	void out_log(std::string message);
	void err_log(std::string message);

	void out_log(int sockfd, std::string message);
	void err_log(int sockfd, std::string message);

	void log_recieved_message(const std::shared_ptr<Message>& message);
};

#endif