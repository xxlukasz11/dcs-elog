#include "connection_handler.h"
#include "config.h"
#include "custom_exceptions.h"
#include "administrator.h"

void Administrator::handle_sigint(int) {
	instance().stop();
}

Administrator::Administrator() : logger_(std::make_shared<Logger>()), database_(config::database::path) {
	signal(SIGINT, &handle_sigint);
	setup_and_start_logger();
}

Administrator& Administrator::instance() {
	static Administrator administrator;
	return administrator;
}

void Administrator::initialize() {
	initialized_ = true;
	setup_server();
	prepare_threads();
}

void Administrator::start() {
	if (!initialized_) {
		Logger::create().location("Administrator::start").error("Attemted to start uninitialized Administartor object");
		return;
	}
	started_ = true;
	thread_manager_.start_servers();
	thread_manager_.start_consumers();

	Logger::create().info("E-log is ready");
	thread_manager_.join_servers();
}

void Administrator::stop() {
	if (!started_) {
		return;
	}
	server_->stop_and_release_consumers();
	thread_manager_.join_consumers();
	on_exit();
	logger_->release();
	thread_manager_.join_loggers();
}

void Administrator::setup_and_start_logger() {
	thread_manager_.add_logger(logger_);
	thread_manager_.start_loggers();
}

void Administrator::setup_server() {
	server_ = std::make_shared<Tcp_server>(socket_queue_);
	server_->set_ip_address(config::server::ip_address);
	server_->set_port(config::server::port);
	server_->set_max_connections(config::connection::max_no_of_connections);
	server_->set_message_length(config::connection::tcp_message_length);
	server_->set_number_of_consumers(config::thread::no_of_connection_handlers);
	server_->set_accept_delay_ms(config::server::accept_delay_ms);
	try {
		server_->initialize();
	}
	catch (Init_server_error& e) {
		Logger::create().error(e.what());
		exit(1);
	}
}

void Administrator::prepare_threads() {
	thread_manager_.add_server(server_);

	int number_of_consumers = config::thread::no_of_connection_handlers;
	for (int i = 0; i < number_of_consumers; ++i) {
		thread_manager_.add_consumer(
			std::make_shared<Connection_handler>(socket_queue_, database_, server_));
	}
}

void Administrator::on_exit() {
	Logger::create().info("E-log finished");
}
