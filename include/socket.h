#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>
#include <arpa/inet.h>

class Socket {
	int socket_descriptor_;
	int message_length_{ 1024 };

public:
	enum class Domain { IPV4 = AF_INET };
	enum class Type { TCP_IP = SOCK_STREAM };
	enum class Protocol { DEFAULT = 0 };
	enum class Descriptor { INVALID = -1 };

	class Addr_in_wrapper {
	public:
		Addr_in_wrapper();
		int set_ip_address(const std::string& ip_address);
		void set_port(int port);
		sockaddr_in& unwrap();
	private:
		sockaddr_in addr_in_;
	};

	Socket(Descriptor descriptor);
	Socket(int descriptor);
	static Socket create(Domain domain, Type type, Protocol protocol);

	int descriptor() const;
	int set_sock_opt(int property, int value);
	int set_reuse_address();
	int set_to_nonblock_mode();
	int bind(Addr_in_wrapper& config);
	int listen(int max_no_of_connections);
	int set_recieve_timeout(int seconds, int u_seconds);
	int set_send_timeout(int seconds, int u_seconds);
	int shutdown_rdwr();
	Socket accept(Addr_in_wrapper& config);

	void set_message_length(int length);
	std::string recv_string();
	void send_string(const std::string& msg);

	template<typename T>
	T receive();

	template<typename Buffer_t>
	void fill_buffer(Buffer_t& buffer, size_t bytes_to_fill);

	template<typename T>
	void send_value(T value);

	bool is_not_valid() const;
	bool is_valid() const;
	bool is_zero() const;
	bool operator<(int number) const;
	bool operator==(int number) const;
	operator int();

private:
	timeval create_time_val(int seconds, int u_seconds);
	int safe_recv(void* buffer, size_t size, int flags);
};

template<typename T>
inline T Socket::receive() {
	T buffer;
	safe_recv(&buffer, sizeof(buffer), 0);
	return buffer;
}

template<typename Buffer_t>
inline void Socket::fill_buffer(Buffer_t& buffer, size_t bytes_to_fill) {
	int received = 0;
	while (received < bytes_to_fill) {
		received += safe_recv(&buffer[received], bytes_to_fill - received, 0);
	}
}

template<typename T>
void Socket::send_value(T value) {
	send(socket_descriptor_, &value, sizeof(value), 0);
}

#endif