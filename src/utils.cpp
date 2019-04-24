#include "utils.h"

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include "custom_exceptions.h"

int utils::safe_recv(int socket, void* buffer, size_t size, int flags) {
	int bytes_read = recv(socket, buffer, size, flags);

	if (bytes_read == 0) {
		throw Client_disconnected_error();
	}
	else if (bytes_read < 0) {
		throw Timeout_error();
	}

	return bytes_read;
}

std::string utils::get_date_time_str(){
	time_t abs_sec = std::time(nullptr);
    tm t_local = *std::localtime(&abs_sec);

    std::ostringstream ss;
    ss << std::put_time(&t_local, "%Y-%m-%d %H:%M:%S");
    
	return ss.str();
}

void utils::out_log(std::string message){
	std::string date = utils::get_date_time_str();
	std::cout << date << "\t" << message << std::endl;
}

void utils::err_log(std::string message){
	std::string date = utils::get_date_time_str();
	std::cerr << date << "\t" << message << std::endl;
}

void utils::out_log(int sockfd, std::string message){
	std::string date = utils::get_date_time_str();
	std::cout << date << "\t" << sockfd << ": " << message << std::endl;
}

void utils::err_log(int sockfd, std::string message){
	std::string date = utils::get_date_time_str();
	std::cerr << date << "\t" << sockfd << ": " << message << std::endl;
}