#include <string>
#include <utility>
#include <sys/socket.h>
#include <sys/types.h>

#include "tcp_server.h"
#include "socket.h"
#include "custom_exceptions.h"
#include "raii_thread.h"
#include "utils.h"
#include "result_set.h"
#include "database.h"
#include "insert_query.h"
#include "select_query.h"
#include "select_tags_query.h"
#include "msg_parser.h"
#include "prepared_statement.h"
#include "json.h"
#include "utils.h"
#include "expand_tree_query.h"
#include "connection_handler.h"

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

void Consumer::process_message(const std::string& message, Socket client_socket){
	Msg_parser parser(message);
	try{
		Connection_handler handler(parser, client_socket);
		handler.handle();

	} catch(Unknown_message_format& e){
		utils::err_log(e.what());
	} catch (Mag_parser_exception& e) {
		utils::err_log(e.what());
	} catch(Database_error& e){
		utils::err_log(e.what());
	} catch(Query_error& e){
		utils::err_log(e.what());
	}
}

void Consumer::insert_data(Msg_parser& parser, Socket client_socket){
	Insert_query query;
	query.set_title(parser.next());
	query.set_desc(parser.next());
	query.set_tags(Msg_parser::extract_tags(parser.next()));
	query.set_author(parser.next());

	std::lock_guard<std::mutex> lock(mtx_);

	Database db("../resources/database.db");
	db.open();

	Result_set res = db.execute( query.create_events_statement() );
	std::string last_id = res.get_last_row_id();
	db.execute( query.create_tags_statements(last_id) );

	client_socket.send_string("Event has been successfully saved");
}

void Consumer::select_data(Msg_parser& parser, Socket client_socket){
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

	auto extracted_tags = Msg_parser::extract_tags(tags_str);
	if (!extracted_tags.empty()) {
		query.set_tags(std::move(extracted_tags));
	}

	Result_set res;
	{
		std::lock_guard<std::mutex> lock(mtx_);
		Database db("../resources/database.db");
		db.open();
		auto stmt = query.create_statement();
		res = db.execute(stmt);
	}
	
	client_socket.send_string(Json::stringify(std::move(res)));
}

void Consumer::return_tags_table(Socket client_socket) {
	Result_set res;
	{
		Select_tags_query query;
		auto stmt = query.create_sql();

		std::lock_guard<std::mutex> lock(mtx_);
		Database db("../resources/database.db");
		db.open();
		res = db.execute(stmt);
	}

	client_socket.send_string(Json::stringify(std::move(res)));
}
