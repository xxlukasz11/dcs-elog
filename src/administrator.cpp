#include "connection_handler.h"
#include "config.h"
#include "custom_exceptions.h"
#include "utils.h"
#include "administrator.h"

void Administrator::handle_sigint(int) {
	instance().stop();
}

Administrator::Administrator() : database_(config::path::database) {
	signal(SIGINT, &handle_sigint);
}

Administrator & Administrator::instance() {
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
		utils::err_log("Administrator::start\tAttemted to start uninitialized Administartor object");
		return;
	}
	started_ = true;
	thread_manager_.start_servers();
	thread_manager_.start_consumers();

	utils::out_log("E-log is ready");
	thread_manager_.join_servers();
}

void Administrator::stop() {
	if (!started_) {
		return;
	}
	server_->stop_and_release_consumers();
	thread_manager_.join_consumers();
	on_exit();
}

void Administrator::setup_server() {
	server_ = std::make_shared<Tcp_server>(socket_queue_);
	server_->set_ip_address("0.0.0.0");
	server_->set_port(9100);
	server_->set_max_connections(50);
	server_->set_message_length(10);
	server_->set_number_of_consumers(3);
	try {
		server_->initialize();
	}
	catch (Init_server_error& e) {
		utils::err_log(e.what());
		exit(1);
	}
}

void Administrator::prepare_threads() {
	thread_manager_.add_server(server_);

	int number_of_consumers = server_->get_number_of_consumers();
	for (int i = 0; i < number_of_consumers; ++i) {
		thread_manager_.add_consumer(
			std::make_shared<Connection_handler>(socket_queue_, database_, server_));
	}
}

void Administrator::on_exit() {
	utils::out_log("E-log has finished");
}
