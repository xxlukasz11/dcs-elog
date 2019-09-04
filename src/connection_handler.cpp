#include <string>
#include <utility>
#include <sys/socket.h>
#include <sys/types.h>

#include "tcp_server.h"
#include "socket.h"
#include "custom_exceptions.h"
#include "raii_thread.h"
#include "utils.h"
#include "message_handler.h"
#include "message_factory.h"
#include "message.h"

#include "connection_handler.h"

Connection_handler::Connection_handler(Socket_queue& queue) {
	thread_ = Raii_thread(&Connection_handler::consume, std::ref(queue));
}

void Connection_handler::join(){
	thread_.join();
}

void configure_socket_timeout(int client_socket, int seconds, int u_seconds){
	struct timeval tv{ seconds, u_seconds };
	if( setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &tv, sizeof(struct timeval)) < 0){
		throw Sockopt_error("Error while setting timeout opt");
	}
}

void Connection_handler::consume(Socket_queue& queue) {
	while(Tcp_server::server_is_running_){
		Socket client_socket = queue.pop();
		
		if(client_socket == Tcp_server::DUMMY_SOCKET_)
			break;
		if(client_socket < 0) {
			utils::err_log(client_socket, "Error while accepting connection");
			continue;
		}
		
		utils::out_log(client_socket, "Connection accepted");

		try{
			configure_socket_timeout(client_socket, Tcp_server::timeout_seconds_, 0);
			
			auto recv_msg = client_socket.recv_string();
			utils::out_log(client_socket, "Message from client: " + recv_msg);

			process_message(recv_msg, client_socket);
		}
		catch(Timeout_error& e){
			utils::err_log(client_socket, e.what());
		}
		catch(Sockopt_error& e){
			utils::err_log(client_socket, e.what());
		}
		
		utils::out_log(client_socket, "Connection closed");
		shutdown(client_socket, SHUT_RDWR);
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
