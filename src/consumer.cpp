#include <string>
#include <utility>
#include <sys/socket.h>
#include <sys/types.h>

#include "tcp_server.h"
#include "custom_exceptions.h"
#include "raii_thread.h"
#include "utils.h"
#include "result_set.h"
#include "database.h"
#include "insert_query.h"
#include "select_query.h"
#include "msg_parser.h"
#include "prepared_statement.h"
#include "json.h"

#include "consumer.h"

std::mutex Consumer::mtx_;

Consumer::Consumer(Socket_queue& queue) {
	thread_ = Raii_thread(&Consumer::consume, std::ref(queue));
}

void Consumer::join(){
	thread_.join();
}

void configure_socket_timeout(int client_socket, int seconds, int u_seconds){
	struct timeval tv{ seconds, u_seconds };
	if( setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &tv, sizeof(struct timeval)) < 0){
		throw Sockopt_error("Error while setting timeout opt");
	}
}

void Consumer::consume(Socket_queue& queue) {
	auto& server = Tcp_server::get_instance();
	while(server.server_is_running_){
		int client_socket = queue.pop();
		
		if(client_socket == Tcp_server::DUMMY_SOCKET_)
			break;
		if(client_socket < 0) {
			utils::err_log(client_socket, "Error while accepting connection");
			continue;
		}
		
		utils::out_log(client_socket, "Connection accepted");

		try{
			configure_socket_timeout(client_socket, server.timeout_seconds_, 0);
			
			
			auto recv_msg = recv_string_from_client(client_socket);
			utils::out_log(client_socket, "Message from client: " + recv_msg);

			process_message(recv_msg, client_socket);
		}
		catch(Timeout_error& e){
			utils::err_log(client_socket, e.what());
		}
		catch(Client_disconnected_error& e){
			utils::err_log(client_socket, e.what());
		}
		catch(Send_error& e){
			utils::err_log(client_socket, e.what());
		}
		catch(Sockopt_error& e){
			utils::err_log(client_socket, e.what());
		}
		
		utils::out_log(client_socket, "Connection closed");

		shutdown(client_socket, SHUT_RDWR);
	}
}

void Consumer::process_message(const std::string& message, const int client_socket){
	Msg_parser parser(message);
	auto mode = parser.get_mode();

	try{
		switch(mode){
			case Msg_parser::mode::insert : insert_data(parser); break;
			case Msg_parser::mode::select : select_data(parser, client_socket); break;

			default: throw Unknown_message_format();
		}

	} catch(Unknown_message_format& e){
		utils::err_log(e.what());
	} catch(Database_error& e){
		utils::err_log(e.what());
	} catch(Query_error& e){
		utils::err_log(e.what());
	}
}

void Consumer::insert_data(Msg_parser& parser){
	Insert_query query;
	query.set_title( parser.next() );
	query.set_desc( parser.next() );
	query.set_tags( Msg_parser::extract_tags( parser.next() ) );

	std::lock_guard<std::mutex> lock(mtx_);

	Database db("../resources/database.db");
	db.open();

	Result_set res = db.execute( query.create_events_statement() );
	std::string last_id = res.get_last_row_id();
	db.execute( query.create_tags_statements(last_id) );

}

void Consumer::select_data(Msg_parser& parser, const int client_socket){
	auto min_date_str = parser.next();
	auto max_date_str = parser.next();
	auto tags_str = parser.next();

	Select_query query;
	if(!min_date_str.empty()){
		query.set_min_date(min_date_str);
	}
	if(!max_date_str.empty()){
		query.set_max_date(max_date_str);
	}
	if(!tags_str.empty()){
		query.set_tags( Msg_parser::extract_tags(tags_str) );
	}

	std::lock_guard<std::mutex> lock(mtx_);
	Database db("../resources/database.db");
	db.open();

	auto stmt = query.create_statement();
	auto res = db.execute(stmt);
	
	send_string_to_client(client_socket, Json::stringify(std::move(res)));
}

std::string Consumer::recv_string_from_client(const int client_socket){
	std::string msg;

	int length = 0;
	recv(client_socket, &length, sizeof(length), 0);

	int total_bytes_read = 0;
	int buffer_size = Tcp_server::get_instance().message_length_;

	std::cout << length << std::endl;

	while (total_bytes_read < length) {
		char recv_buffer[buffer_size + 1];
		int bytes_read = recv(client_socket, recv_buffer, buffer_size, 0);

		if (bytes_read == 0) {
			throw Client_disconnected_error();
		}
		else if (bytes_read < 0) {
			throw Timeout_error();
		}
		std::cout << bytes_read << std::endl;

		total_bytes_read += bytes_read;
		recv_buffer[bytes_read] = '\0';
		msg += recv_buffer;
	}


	return msg;
}

void Consumer::send_string_to_client(const int client_socket, const std::string & msg) {
	int length = msg.size();
	int err_code = send(client_socket, &length, 4, 0);

	if (err_code <= 0) {
		throw Send_error();
	}

	int total_bytes_sent = 0;
	const char* buffer = msg.c_str();
	int max_buffer_size = Tcp_server::get_instance().message_length_;

	while (total_bytes_sent < length) {
		int remaining_bytes = length - total_bytes_sent;
		int packet_size = (max_buffer_size < remaining_bytes) ? max_buffer_size : remaining_bytes;
		int bytes_sent = send(client_socket, buffer + total_bytes_sent, packet_size, 0);

		if (bytes_sent <= 0) {
			throw Send_error();
		}

		total_bytes_sent += bytes_sent;
	}

}
