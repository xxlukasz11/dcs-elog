#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include "custom_exceptions.h"
#include <vector>
#include "socket.h"

Socket::Socket(Descriptor descriptor) : Socket(static_cast<int>(descriptor)) {
}

Socket::Socket(int descriptor) : socket_descriptor_(descriptor) {
}

int Socket::descriptor() const {
	return socket_descriptor_;
}

void Socket::set_message_length(int length) {
	message_length_ = length;
}

int Socket::set_sock_opt(int opt_name, int opt_value) {
	int opt = 1;
	return ::setsockopt(socket_descriptor_, opt_name, opt_value, &opt, sizeof(opt));
}

int Socket::set_reuse_address() {
	return set_sock_opt(SOL_SOCKET, SO_REUSEADDR);
}

int Socket::set_to_nonblock_mode() {
	return ::fcntl(socket_descriptor_, F_SETFL, O_NONBLOCK);
}

// disable SIGPIPE signal
void Socket::set_no_signal() {
	send_flags_ |= MSG_NOSIGNAL;
	recv_flags_ |= MSG_NOSIGNAL;
}

int Socket::bind(Addr_in_wrapper& config) {
	sockaddr_in& addr_in = config.unwrap();
	return ::bind(socket_descriptor_, (struct sockaddr*) &addr_in, sizeof(addr_in));
}

int Socket::listen(int max_no_of_connections) {
	return ::listen(socket_descriptor_, max_no_of_connections);
}

int Socket::set_recieve_timeout(int seconds, int u_seconds) {
	timeval time_struct = create_time_val(seconds, u_seconds);
	return setsockopt(socket_descriptor_, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &time_struct, sizeof(struct timeval));
}

int Socket::set_send_timeout(int seconds, int u_seconds) {
	timeval time_struct = create_time_val(seconds, u_seconds);
	return setsockopt(socket_descriptor_, SOL_SOCKET, SO_SNDTIMEO, (struct timeval*) &time_struct, sizeof(struct timeval));
}

int Socket::shutdown_rdwr() {
	return shutdown(socket_descriptor_, SHUT_RDWR);
}

Socket Socket::accept(Addr_in_wrapper& config) {
	sockaddr_in& addr_in = config.unwrap();
	socklen_t length = sizeof(addr_in);
	return ::accept(socket_descriptor_, (struct sockaddr *) &addr_in, &length);
}

Socket Socket::create(Domain domain, Type type, Protocol protocol) {
	return socket(
		static_cast<int>(domain),
		static_cast<int>(type),
		static_cast<int>(protocol));
}

std::string Socket::recv_string() {
	const int length = receive<int>();
	std::string msg;
	int total_bytes_read = 0;
	std::vector<char> recv_buffer;
	while (total_bytes_read < length) {
		const int current_buffer_size = std::min(message_length_, length - total_bytes_read);
		recv_buffer.resize(current_buffer_size + 1);
		const int bytes_read = safe_recv(recv_buffer.data(), current_buffer_size);

		total_bytes_read += bytes_read;
		recv_buffer[bytes_read] = '\0';
		msg += recv_buffer.data();
	}
	return msg;
}

void Socket::send_string(const std::string& msg) {
	const int length = msg.size();
	int total_bytes_sent = 0;
	const char* buffer = msg.c_str();
	const int max_buffer_size = message_length_;

	while (total_bytes_sent < length) {
		const int remaining_bytes = length - total_bytes_sent;
		const int packet_size = (max_buffer_size < remaining_bytes) ? max_buffer_size : remaining_bytes;
		const int bytes_sent = safe_send(buffer + total_bytes_sent, packet_size);

		if (bytes_sent <= 0) {
			throw Send_error();
		}

		total_bytes_sent += bytes_sent;
	}
}

bool Socket::is_not_valid() const {
	return socket_descriptor_ < 0;
}

bool Socket::is_valid() const {
	return socket_descriptor_ > 0;
}

bool Socket::is_zero() const {
	return socket_descriptor_ == 0;
}

bool Socket::operator<(int number) const {
	return socket_descriptor_ < number;
}

bool Socket::operator==(int number) const {
	return socket_descriptor_ == number;
}

Socket::operator int() {
	return socket_descriptor_;
}

timeval Socket::create_time_val(int seconds, int u_seconds) {
	return timeval { seconds, u_seconds };
}

int Socket::safe_recv(void * buffer, size_t size) {
	const int bytes_read = recv(socket_descriptor_, buffer, size, recv_flags_);

	if (bytes_read == 0) {
		throw Client_disconnected_error();
	}
	else if (bytes_read < 0) {
		throw Timeout_error();
	}

	return bytes_read;
}

int Socket::safe_send(const void* buffer, size_t size) {
	const int bytes_sent = send(socket_descriptor_, buffer, size, send_flags_);

	if (bytes_sent <= 0) {
		throw Send_error();
	}

	return bytes_sent;
}

Socket::Addr_in_wrapper::Addr_in_wrapper() {
	addr_in_.sin_family = AF_INET;
}

int Socket::Addr_in_wrapper::set_ip_address(const std::string& ip_address) {
	return ::inet_pton(addr_in_.sin_family, ip_address.c_str(), &addr_in_.sin_addr);
}

void Socket::Addr_in_wrapper::set_port(int port) {
	addr_in_.sin_port = htons(port);
}

sockaddr_in& Socket::Addr_in_wrapper::unwrap() {
	return addr_in_;
}
