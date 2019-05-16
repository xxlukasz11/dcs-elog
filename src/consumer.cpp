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
#include "utils.h"
#include "expand_tree_query.h"

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
			
			
			auto recv_msg = utils::recv_string_from_client(client_socket);
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

void Consumer::process_message(const std::string& message, int client_socket){
	Msg_parser parser(message);
	auto mode = parser.get_mode();

	try{
		switch(mode){
			case Msg_parser::mode::insert : insert_data(parser, client_socket); break;
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

void Consumer::insert_data(Msg_parser& parser, int client_socket){
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

	utils::send_string_to_client(client_socket, "Data has been recievied");
}

void Consumer::select_data(Msg_parser& parser, int client_socket){
	auto min_date_str = parser.next();
	auto max_date_str = parser.next();
	auto tags_str = parser.next();

	std::vector<std::string> tags_vector;

	if (!tags_str.empty()) {
		auto extracted_tags = Msg_parser::extract_tags(tags_str);
		Expand_tree_query exp_query(extracted_tags);
		auto stmt = exp_query.create_statement();

		Result_set res;
		{
			std::lock_guard<std::mutex> lock(mtx_);
			Database db("../resources/database.db");
			db.open();
			res = db.execute(stmt);
		}
		
		tags_vector = res.get_column(0);
	}

	Select_query query;
	if(!min_date_str.empty()){
		query.set_min_date(min_date_str);
	}
	if(!max_date_str.empty()){
		query.set_max_date(max_date_str);
	}
	if(!tags_vector.empty()){
		query.set_tags( std::move(tags_vector) );
	}

	Result_set res;
	{
		std::lock_guard<std::mutex> lock(mtx_);
		Database db("../resources/database.db");
		db.open();
		auto stmt = query.create_statement();
		res = db.execute(stmt);
	}
	
	utils::send_string_to_client(client_socket, Json::stringify(std::move(res)));
}
