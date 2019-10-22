#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <string>
#include <chrono>
#include "log_level.h"

class Parameters {
public:
	int get_no_of_connection_handlers() const;
	int get_tcp_message_length() const;
	int get_max_no_of_connections() const;
	int get_receive_timeout_seconds() const;
	int get_listening_port() const;
	int get_connection_accept_delay_ms() const;
	const std::string& get_local_ip_address() const;
	const std::string& get_database_path() const;
	const std::string& get_empty_tag_name() const;
	const std::string& get_log_file_path() const;
	std::chrono::seconds get_logger_inactivity_timeout() const;
	Log_level get_log_level() const;

	void set_no_of_connection_handlers(int no_of_connection_handlers);
	void set_tcp_message_length(int tcp_message_length);
	void set_max_no_of_connections(int max_no_of_connections);
	void set_receive_timeout_seconds(int receive_timeout_seconds);
	void set_listening_port(int listening_port);
	void set_connection_accept_delay_ms(int connection_accept_delay_ms);
	void set_local_ip_address(const std::string& local_ip_address);
	void set_database_path(const std::string& database_path);
	void set_empty_tag_name(const std::string& empty_tag_name);
	void set_log_file_path(const std::string& log_file_path);
	void set_logger_inactivity_timeout(std::chrono::seconds logger_inactivity_timeout);
	void set_log_level(Log_level log_level);

private:
	int no_of_connection_handlers_;
	int tcp_message_length_;
	int max_no_of_connections_;
	int receive_timeout_seconds_;
	int listening_port_;
	int connection_accept_delay_ms_;
	std::string local_ip_address_;
	std::string database_path_;
	std::string empty_tag_name_;
	std::string log_file_path_;
	std::chrono::seconds logger_inactivity_timeout_;
	Log_level log_level_;
};

#endif // !_PARAMETERS_H_
