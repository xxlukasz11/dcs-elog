#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <atomic>
#include <cstdint>
#include "concurrent_queue.h"
#include "thread_base.h"

/*
Listens to incoming connections, pushes them to queue and wakes up one ofthe connection handlers
*/
class Tcp_server : public Thread_base {
public:
	Tcp_server(Socket_queue& socket_queue);
	virtual ~Tcp_server();
	Tcp_server(const Tcp_server&) = delete;
	Tcp_server(Tcp_server&&) = delete;
	Tcp_server& operator=(const Tcp_server&) = delete;
	Tcp_server& operator=(Tcp_server&&) = delete;

	// setters

	void set_ip_address(const std::string& ip_address);
	void set_port(int port);
	void set_message_length(int length);
	void set_max_connections(int max_connections);
	void set_recieve_timeout_seconds(int seconds);
	void set_number_of_consumers(int number_of_consumers);
	void set_accept_delay_ms(int accept_delay_ms);

	// getters

	Socket_queue& get_socket_queue();
	const std::atomic<bool>& get_running_flag() const;
	std::string get_ip_address() const;
	int get_port() const;
	int get_message_length() const;
	int get_max_connections() const;
	int get_recieve_timeout_seconds() const;
	int get_number_of_consumers() const;
	int get_accept_delay_ms() const;

	// public methods

	void initialize();
	void stop_and_release_consumers();

	enum Constants : int {
		DUMMY_SOCKET_ = -0xFFFFFFF
	};

private:
	// private methods

	virtual void run();
	void close_socket_if_valid();

	// configuration

	std::string ip_address_{ "0.0.0.0" };
	int port_;
	int message_length_;
	int max_connections_;
	int recieve_timeout_seconds_;
	int number_of_consumers_;
	int accept_delay_ms_;

	// other private members

	Socket server_socket_{ Socket::Descriptor::INVALID };
	Socket_queue& queue_;
	std::atomic<bool> server_is_running_{ false };
};

#endif