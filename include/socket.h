#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>

class Socket {
	int socket_descriptor_;
	int message_length_{ 1024 };

public:
	Socket(int descriptor);
	int descriptor() const;
	void set_message_length(int length);

	std::string recv_string();
	void send_string(const std::string & msg);

	bool operator<(int number) const;
	bool operator==(int number) const;
	operator int();

private:
	int safe_recv(void* buffer, size_t size, int flags);
};

#endif