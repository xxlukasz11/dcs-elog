#include "connection_handler.h"
#include "custom_exceptions.h"
#include "param_manager.h"
#include "administrator.h"

void Administrator::handle_sigint(int) {
	instance().stop();
}

Administrator::Administrator() {
	signal(SIGINT, &handle_sigint);
}

Administrator& Administrator::instance() {
	static Administrator administrator;
	return administrator;
}

const Parameters& Administrator::params() const {
	return parameters_;
}

void Administrator::initialize(const Parameters& parameters) {
	parameters_ = parameters;
	initialized_ = true;
	create_and_start_logger();
	create_database();
	setup_server();
	prepare_threads();
}

void Administrator::start() {
	if (!initialized_) {
		Logger::create().level(Log_level::CRITICAL)
			.error("Attemted to start Administartor without initialization");
		return;
	}
	started_ = true;
	thread_manager_.start_servers();
	thread_manager_.start_consumers();

	Logger::create().level(Log_level::CRITICAL).info("E-log is ready");
	thread_manager_.join_servers();
}

void Administrator::stop() {
	if (!started_) {
		return;
	}
	server_->stop_and_release_consumers();
	thread_manager_.join_consumers();
	on_exit();
	stop_logger();
}

void Administrator::create_database() {
	database_ = std::make_unique<Database>(parameters_.get_database_path());
}

void Administrator::stop_logger() {
	logger_->release();
	thread_manager_.join_loggers();
}

void Administrator::create_and_start_logger() {
	logger_ = std::make_shared<Logger>(parameters_.get_log_file_path());
	logger_->set_timeout(parameters_.get_logger_inactivity_timeout());
	logger_->set_log_level(parameters_.get_log_level());
	thread_manager_.add_logger(logger_);
	thread_manager_.start_loggers();
}

void Administrator::setup_server() {
	server_ = std::make_shared<Tcp_server>(socket_queue_);
	server_->set_ip_address(parameters_.get_local_ip_address());
	server_->set_port(parameters_.get_listening_port());
	server_->set_max_connections(parameters_.get_max_no_of_connections());
	server_->set_message_length(parameters_.get_tcp_message_length());
	server_->set_number_of_consumers(parameters_.get_no_of_connection_handlers());
	server_->set_accept_delay_ms(parameters_.get_connection_accept_delay_ms());
	try {
		server_->initialize();
	}
	catch (const Init_server_error& e) {
		Logger::create().level(Log_level::CRITICAL).error(e.what());
		exit(1);
	}
}

void Administrator::prepare_threads() {
	thread_manager_.add_server(server_);

	const int number_of_consumers = parameters_.get_no_of_connection_handlers();
	for (int i = 0; i < number_of_consumers; ++i) {
		thread_manager_.add_consumer(
			std::make_shared<Connection_handler>(socket_queue_, *database_, server_));
	}
}

void Administrator::on_exit() {
	Logger::create().level(Log_level::CRITICAL).info("E-log finished");
}
