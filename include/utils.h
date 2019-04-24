#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

namespace utils{
	int safe_recv(int socket, void* buffer, size_t size, int flags);
	std::string recv_string_from_client(int client_socket);
	void send_string_to_client(int client_socket, const std::string & msg);

	std::string get_date_time_str();
	void out_log(std::string message);
	void err_log(std::string message);

	void out_log(int sockfd, std::string message);
	void err_log(int sockfd, std::string message);
};

#endif