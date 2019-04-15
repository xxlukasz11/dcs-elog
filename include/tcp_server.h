#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <atomic>

#include "socket_queue.h"
#include "thread_pool.h"

class Consumer;

class Tcp_server {
	friend class Consumer;
public:
	Tcp_server(const Tcp_server&) = delete;
	Tcp_server(Tcp_server&&) = delete;
	Tcp_server& operator=(const Tcp_server&) = delete;
	Tcp_server& operator=(Tcp_server&&) = delete;

	// get instance method
	static Tcp_server& get_instance();

	// setters
	void set_ip_address(std::string ip_address);
	void set_port(int port);
	void set_message_length(int length);
	void set_max_connections(int max_connsctions);
	void set_timeout_seconds(int seconds);
	void set_number_of_consumers(int n_consumers);

	// public methods
	void initialize();
	void start_server();
	void join_threads();

private:
	// private constructor
	Tcp_server();
	~Tcp_server();

	// private methods
	void run_server();
	void release_consumers();
	static void set_stop_variable(int);

	enum Constants {
		DUMMY_SOCKET_ = -123456,
		ACCEPT_DELAY_MS = 50,
	};

	// configuration
	std::string ip_address_{ "0.0.0.0" };
	int port_ = { 9100 };
	int message_length_{ 1024 };
	int max_connections_{ 50 };
	int timeout_seconds_{ 5 };
	int number_of_consumers_{ 3 };

	// private members
	int server_socket_{ 0 };
	Socket_queue queue_;
	Thread_pool threads_manager_;
	static std::atomic<bool> server_is_running_;
};

#endif