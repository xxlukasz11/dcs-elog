#include <string>
#include <memory>
#include "thread_base.h"
#include "tcp_server.h"
#include "socket.h"
#include "custom_exceptions.h"
#include "message_handler.h"
#include "logger.h"
#include "connection_handler.h"

namespace {
	bool is_dummy_socket(const Socket& socket) {
		return socket == Tcp_server::DUMMY_SOCKET_;
	}
}

Connection_handler::Connection_handler(Socket_queue& queue, Database& database, const std::shared_ptr<Tcp_server>& server)
	: Thread_base(), queue_(queue), database_(database), server_(server), socket_(Socket::Descriptor::INVALID) {
}

void Connection_handler::run() {
	while(server_->get_running_flag().load()) {
		socket_ = queue_.pop();
		
		if (is_dummy_socket(socket_)) {
			break;
		}

		Logger::create().context(socket_).level(Log_level::BASIC).info("Connection accepted");
		handle_connection();
		Logger::create().context(socket_).level(Log_level::BASIC).info("Connection closed");
		socket_.shutdown_rdwr();
	}
}

void Connection_handler::handle_connection() {
	if (socket_.set_recieve_timeout(server_->get_recieve_timeout_seconds(), 0) < 0) {
		Logger::create().context(socket_).level(Log_level::WARNING).error("Error while setting recieve timeout opt");
		return;
	}
	socket_.set_no_signal();
	recieve_data_from_socket();
}

void Connection_handler::recieve_data_from_socket() {
	try {
		auto recv_msg = socket_.recv_string();
		Logger::create().context(socket_).level(Log_level::ALL).info("Recieved payload: " + recv_msg);
		Message_handler handler(socket_, database_);
		handler.process_message(recv_msg);
	}
	catch (const Timeout_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).warning(e.what());
	}
	catch (const Client_disconnected_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).warning(e.what());
	}
	catch (const Send_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	}
}
