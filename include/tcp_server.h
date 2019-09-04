#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <atomic>

#include "socket_queue.h"
#include "dcs_thread.h"

class Tcp_server : public Dcs_thread {
public:
	Tcp_server(Socket_queue& socket_queue);
	~Tcp_server();
	Tcp_server(const Tcp_server&) = delete;
	Tcp_server(Tcp_server&&) = delete;
	Tcp_server& operator=(const Tcp_server&) = delete;
	Tcp_server& operator=(Tcp_server&&) = delete;

	// setters
	void set_ip_address(std::string ip_address);
	void set_port(int port);
	void set_message_length(int length);
	void set_max_connections(int max_connsctions);
	void set_timeout_seconds(int seconds);
	void set_number_of_consumers(int n_consumers);

	// getters
	Socket_queue& get_socket_queue();
	const std::atomic<bool>& get_running_flag() const;
	std::string get_ip_address() const;
	int get_port() const;
	int get_message_length() const;
	int get_max_connections() const;
	int get_timeout_seconds() const;
	int get_number_of_consumers() const;

	// public methods
	void initialize();
	void release_consumers();

	enum Constants {
		DUMMY_SOCKET_ = -123456,
		ACCEPT_DELAY_MS = 50,
	};

private:
	// private methods
	virtual void run();

	// configuration
	std::string ip_address_{ "0.0.0.0" };
	int port_{ 9100 };
	int message_length_{ 1024 };
	int max_connections_{ 50 };
	int timeout_seconds_{ 5 };
	int number_of_consumers_{ 3 };

	// other private members
	int server_socket_{ 0 };
	Socket_queue& queue_;
	std::atomic<bool> server_is_running_{ false };
};

#endif