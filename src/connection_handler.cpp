#include <string>
#include <memory>
#include "tcp_server.h"
#include "socket.h"
#include "custom_exceptions.h"
#include "utils.h"
#include "message_handler.h"
#include "message_factory.h"
#include "message.h"

#include "connection_handler.h"

namespace {
	bool is_dummy_socket(const Socket& socket) {
		return socket == Tcp_server::DUMMY_SOCKET_;
	}
}

Connection_handler::Connection_handler(Socket_queue& queue, const std::shared_ptr<Tcp_server>& server)
	: Dcs_thread(), queue_(queue), server_(server), socket_(Socket::Descriptor::INVALID) {
}

void Connection_handler::run() {
	while(server_->get_running_flag().load()) {
		socket_ = queue_.pop();
		
		if (is_dummy_socket(socket_)) {
			break;
		}
		if (socket_.is_not_valid()) {
			utils::err_log(socket_, "Error while accepting connection");
			continue;
		}

		utils::out_log(socket_, "Connected");
		handle_connection();
		utils::out_log(socket_, "Connection closed");
		socket_.shutdown_rdwr();
	}
}

void Connection_handler::handle_connection() {
	if (socket_.set_recieve_timeout(server_->get_recieve_timeout_seconds(), 0) < 0) {
		utils::err_log(socket_, "Error while setting recieve timeout opt");
		return;
	}
	recieve_data_from_socket();
}

void Connection_handler::recieve_data_from_socket() {
	try {
		auto recv_msg = socket_.recv_string();
		utils::out_log(socket_, "Message contents: " + recv_msg);
		process_message(recv_msg);
	}
	catch (Timeout_error& e) {
		utils::err_log(socket_, e.what());
	}
}

void Connection_handler::process_message(const std::string& message_string){
	try{
		Message_factory factory(message_string);
		auto internal_message = factory.create();
		utils::log_recieved_message(socket_, internal_message);

		Message_handler handler(socket_);
		handler.handle(internal_message);

	} catch(Unknown_message_format& e){
		utils::err_log(e.what());
	} catch (Unknown_message& e) {
		utils::err_log(e.what());
	} catch (Msg_parser_exception& e) {
		utils::err_log(e.what());
	} catch(Database_error& e){
		utils::err_log(e.what());
	} catch(Query_error& e){
		utils::err_log(e.what());
	} catch (Client_disconnected_error& e) {
		utils::err_log(socket_, e.what());
	} catch (Send_error& e) {
		utils::err_log(socket_, e.what());
	}
}
