#include <string>
#include <memory>
#include "tcp_server.h"
#include "socket.h"
#include "custom_exceptions.h"
#include "message_handler.h"
#include "message_factory.h"
#include "message.h"
#include "logger.h"

#include "connection_handler.h"

namespace {
	bool is_dummy_socket(const Socket& socket) {
		return socket == Tcp_server::DUMMY_SOCKET_;
	}
}

Connection_handler::Connection_handler(Socket_queue& queue, Database& database, const std::shared_ptr<Tcp_server>& server)
	: Dcs_thread(), queue_(queue), database_(database), server_(server), socket_(Socket::Descriptor::INVALID) {
}

void Connection_handler::run() {
	while(server_->get_running_flag().load()) {
		socket_ = queue_.pop();
		
		if (is_dummy_socket(socket_)) {
			break;
		}
		if (socket_.is_not_valid()) {
			Logger::create().context(socket_).error("Error while accepting connection");
			continue;
		}

		Logger::create().context(socket_).info("Connection accepted");
		handle_connection();
		Logger::create().context(socket_).info("Connection closed");
		socket_.shutdown_rdwr();
	}
}

void Connection_handler::handle_connection() {
	if (socket_.set_recieve_timeout(server_->get_recieve_timeout_seconds(), 0) < 0) {
		Logger::create().context(socket_).error("Error while setting recieve timeout opt");
		return;
	}
	recieve_data_from_socket();
}

void Connection_handler::recieve_data_from_socket() {
	try {
		auto recv_msg = socket_.recv_string();
		Logger::create().context(socket_).info("Recieved payload: " + recv_msg);
		process_message(recv_msg);
	}
	catch (Timeout_error& e) {
		Logger::create().context(socket_).warning(e.what());
	}
	catch (Client_disconnected_error& e) {
		Logger::create().context(socket_).warning(e.what());
	}
}

void Connection_handler::process_message(const std::string& message_string){
	try{
		Message_factory factory(message_string);
		auto internal_message = factory.create();
		Logger::create().context(socket_).info(internal_message);

		Message_handler handler(socket_, database_);
		handler.handle(internal_message);

	} catch(Unknown_message_format& e){
		Logger::create().context(socket_).error(e.what());
	} catch (Unknown_message& e) {
		Logger::create().context(socket_).error(e.what());
	} catch(Database_error& e){
		Logger::create().context(socket_).error(e.what());
	} catch(Query_error& e){
		Logger::create().context(socket_).error(e.what());
	} catch (Send_error& e) {
		Logger::create().context(socket_).error(e.what());
	}
}
