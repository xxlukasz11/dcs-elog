#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <atomic>

#include "socket_queue.h"
#include "thread_pool.h"

class Connection_handler;

class Tcp_server {
	friend class Connection_handler;
public:
	Tcp_server(const Tcp_server&) = delete;
	Tcp_server(Tcp_server&&) = delete;
	Tcp_server& operator=(const Tcp_server&) = delete;
	Tcp_server& operator=(Tcp_server&&) = delete;

	// get instance method
	static Tcp_server& get_instance();

	// setters
	static void set_ip_address(std::string ip_address);
	static void set_port(int port);
	static void set_message_length(int length);
	static void set_max_connections(int max_connsctions);
	static void set_timeout_seconds(int seconds);
	static void set_number_of_consumers(int n_consumers);

	// getters
	static int get_message_length();

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
	static std::string ip_address_;
	static int port_;
	static int message_length_;
	static int max_connections_;
	static int timeout_seconds_;
	static int number_of_consumers_;

	// other private members
	int server_socket_{ 0 };
	Socket_queue queue_;
	Thread_pool threads_manager_;
	static std::atomic<bool> server_is_running_;
};

#endif