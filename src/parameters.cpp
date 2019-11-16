#include "parameters.h"

int Parameters::get_no_of_connection_handlers() const {
	return no_of_connection_handlers_;
}

int Parameters::get_tcp_message_length() const {
	return tcp_message_length_;
}

int Parameters::get_max_no_of_connections() const {
	return max_no_of_connections_;
}

int Parameters::get_receive_timeout_seconds() const {
	return receive_timeout_seconds_;
}

int Parameters::get_listening_port() const {
	return listening_port_;
}

int Parameters::get_connection_accept_delay_ms() const {
	return connection_accept_delay_ms_;
}

const std::string& Parameters::get_local_ip_address() const {
	return local_ip_address_;
}

const std::string& Parameters::get_database_path() const {
	return database_path_;
}

const std::string& Parameters::get_attachment_storage_path() const {
	return attachment_storage_path_;
}

const std::string& Parameters::get_empty_tag_name() const {
	return empty_tag_name_;
}

const std::string& Parameters::get_log_file_path() const {
	return log_file_path_;
}

std::chrono::seconds Parameters::get_logger_inactivity_timeout() const {
	return logger_inactivity_timeout_;
}

Log_level Parameters::get_log_level() const {
	return log_level_;
}

void Parameters::set_no_of_connection_handlers(int no_of_connection_handlers) {
	no_of_connection_handlers_ = no_of_connection_handlers;
}

void Parameters::set_tcp_message_length(int tcp_message_length) {
	tcp_message_length_ = tcp_message_length;
}

void Parameters::set_max_no_of_connections(int max_no_of_connections) {
	max_no_of_connections_ = max_no_of_connections;
}

void Parameters::set_receive_timeout_seconds(int receive_timeout_seconds) {
	receive_timeout_seconds_ = receive_timeout_seconds;
}

void Parameters::set_listening_port(int listening_port) {
	listening_port_ = listening_port;
}

void Parameters::set_connection_accept_delay_ms(int connection_accept_delay_ms) {
	connection_accept_delay_ms_ = connection_accept_delay_ms;
}

void Parameters::set_local_ip_address(const std::string& local_ip_address) {
	local_ip_address_ = local_ip_address;
}

void Parameters::set_database_path(const std::string& database_path) {
	database_path_ = database_path;
}

void Parameters::set_attachment_storage_path(const std::string& attachment_storage_path) {
	attachment_storage_path_ = attachment_storage_path;
}

void Parameters::set_empty_tag_name(const std::string& empty_tag_name) {
	empty_tag_name_ = empty_tag_name;
}

void Parameters::set_log_file_path(const std::string& log_file_path) {
	log_file_path_ = log_file_path;
}

void Parameters::set_logger_inactivity_timeout(std::chrono::seconds logger_inactivity_timeout) {
	logger_inactivity_timeout_ = logger_inactivity_timeout;
}

void Parameters::set_log_level(Log_level log_level) {
	log_level_ = log_level;
}
