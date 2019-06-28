#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

namespace utils{
	std::string get_date_time_str();
	std::string concatenate_string_array(const std::vector<std::string>& array);

	void out_log(std::string message);
	void err_log(std::string message);

	void out_log(int sockfd, std::string message);
	void err_log(int sockfd, std::string message);
};

#endif