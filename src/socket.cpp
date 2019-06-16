#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include "custom_exceptions.h"
#include "tcp_server.h"

Socket::Socket(int descriptor) : socket_descriptor_(descriptor) {
}

int Socket::get_descriptor() const {
	return socket_descriptor_;
}

std::string Socket::recv_string() {
	std::string msg;

	int length = 0;
	safe_recv(&length, sizeof(length), 0);

	int total_bytes_read = 0;
	int buffer_size = Tcp_server::get_instance().get_message_length();

	while (total_bytes_read < length) {
		char recv_buffer[buffer_size + 1];
		int bytes_read = safe_recv(recv_buffer, buffer_size, 0);

		total_bytes_read += bytes_read;
		recv_buffer[bytes_read] = '\0';
		msg += recv_buffer;
	}

	return msg;
}

void Socket::send_string(const std::string & msg) {
	int length = msg.size();
	int total_bytes_sent = 0;
	const char* buffer = msg.c_str();
	int max_buffer_size = Tcp_server::get_instance().get_message_length();

	while (total_bytes_sent < length) {
		int remaining_bytes = length - total_bytes_sent;
		int packet_size = (max_buffer_size < remaining_bytes) ? max_buffer_size : remaining_bytes;
		int bytes_sent = send(socket_descriptor_, buffer + total_bytes_sent, packet_size, 0);

		if (bytes_sent <= 0) {
			throw Send_error();
		}

		total_bytes_sent += bytes_sent;
	}
}

int Socket::safe_recv(void * buffer, size_t size, int flags) {
	int bytes_read = recv(socket_descriptor_, buffer, size, flags);

	if (bytes_read == 0) {
		throw Client_disconnected_error();
	}
	else if (bytes_read < 0) {
		throw Timeout_error();
	}

	return bytes_read;
}
