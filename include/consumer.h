#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <string>
#include <mutex>

#include "raii_thread.h"
#include "socket_queue.h"
#include "msg_parser.h"

class Consumer {
public:
	Consumer(Socket_queue& queue);
	
	void join();

private:
	// functions used to handle connection
	static void consume(Socket_queue& queue);
	static void process_message(const std::string& message, int client_socket);

	static void insert_data(Msg_parser& query, int client_socket);
	static void select_data(Msg_parser& query, int client_socket);
	static void return_tags_table(int client_socket);

	// private members
	static std::mutex mtx_;
	Raii_thread thread_;
};


#endif
