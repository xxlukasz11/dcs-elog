#include "utils.h"

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include "custom_exceptions.h"
#include "tcp_server.h"

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

std::string utils::recv_string_from_client(int client_socket) {
	std::string msg;

	int length = 0;
	utils::safe_recv(client_socket, &length, sizeof(length), 0);

	int total_bytes_read = 0;
	int buffer_size = Tcp_server::get_instance().get_message_length();

	while (total_bytes_read < length) {
		char recv_buffer[buffer_size + 1];
		int bytes_read = utils::safe_recv(client_socket, recv_buffer, buffer_size, 0);

		total_bytes_read += bytes_read;
		recv_buffer[bytes_read] = '\0';
		msg += recv_buffer;
	}


	return msg;
}

void utils::send_string_to_client(int client_socket, const std::string & msg) {
	int length = msg.size();
	int total_bytes_sent = 0;
	const char* buffer = msg.c_str();
	int max_buffer_size = Tcp_server::get_instance().get_message_length();

	while (total_bytes_sent < length) {
		int remaining_bytes = length - total_bytes_sent;
		int packet_size = (max_buffer_size < remaining_bytes) ? max_buffer_size : remaining_bytes;
		int bytes_sent = send(client_socket, buffer + total_bytes_sent, packet_size, 0);

		if (bytes_sent <= 0) {
			throw Send_error();
		}

		total_bytes_sent += bytes_sent;
	}

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