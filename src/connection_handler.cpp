#include <string>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>

#include "tcp_server.h"
#include "socket.h"
#include "custom_exceptions.h"
#include "utils.h"
#include "message_handler.h"
#include "message_factory.h"
#include "message.h"

#include "connection_handler.h"

namespace {
	bool is_dummy_socket(Socket socket) {
		return socket == Tcp_server::DUMMY_SOCKET_;
	}

	bool socket_is_not_valid(Socket socket) {
		return socket < 0;
	}
}

Connection_handler::Connection_handler(Socket_queue& queue, const std::shared_ptr<Tcp_server>& server) : Dcs_thread(), queue_(queue), server_(server) {
}

void configure_socket_timeout(int client_socket, int seconds, int u_seconds){
	struct timeval tv{ seconds, u_seconds };
	if( setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &tv, sizeof(struct timeval)) < 0){
		throw Sockopt_error("Error while setting timeout opt");
	}
}

void Connection_handler::run() {
	while(server_->get_running_flag().load()) {
		Socket client_socket = queue_.pop();
		
		if (is_dummy_socket(client_socket)) {
			break;
		}
		if (socket_is_not_valid(client_socket)) {
			utils::err_log(client_socket, "Error while accepting connection");
			continue;
		}

		handle_connection(client_socket);
		utils::out_log(client_socket, "Connection closed");
		shutdown(client_socket, SHUT_RDWR);
	}
}

void Connection_handler::handle_connection(Socket socket) {
	try {
		configure_socket_timeout(socket, server_->get_timeout_seconds(), 0);
	}
	catch (Sockopt_error& e) {
		utils::err_log(socket, e.what());
		return;
	}

	recieve_data_from_socket(socket);
}

void Connection_handler::recieve_data_from_socket(Socket socket) {
	try {
		auto recv_msg = socket.recv_string();
		utils::out_log(socket, "Message from client: " + recv_msg);
		process_message(recv_msg, socket);
	}
	catch (Timeout_error& e) {
		utils::err_log(socket, e.what());
		return;
	}
}

void Connection_handler::process_message(const std::string& message_string, Socket client_socket){
	try{
		Message_factory factory(message_string);
		auto internal_message = factory.create();
		Message_handler handler(client_socket);
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
		utils::err_log(client_socket, e.what());
	} catch (Send_error& e) {
		utils::err_log(client_socket, e.what());
	}
}
