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
	static void process_message(const std::string& message, Socket client_socket);

	// private members
	static std::mutex mtx_;
	Raii_thread thread_;
};


#endif
