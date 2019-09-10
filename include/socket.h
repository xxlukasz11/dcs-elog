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
	int set_to_nonblock_mode();
	int bind(Addr_in_wrapper& config);
	int listen(int max_no_of_connections);
	int set_recieve_timeout(int seconds, int u_seconds);
	int set_send_timeout(int seconds, int u_seconds);
	int shutdown_rdwr();
	Socket accept(Addr_in_wrapper& config);

	void set_message_length(int length);
	std::string recv_string();
	void send_string(const std::string & msg);

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

#endif