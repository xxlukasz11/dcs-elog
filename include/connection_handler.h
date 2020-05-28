#ifndef _CONNECTION_HANDLER_H_
#define _CONNECTION_HANDLER_H_

#include "concurrent_queue.h"
#include "database.h"
#include "msg_parser.h"
#include "thread_base.h"
#include "socket.h"

class Tcp_server;

/*
Accepts connection and receives first message
*/
class Connection_handler : public Thread_base {
public:
	Connection_handler(Socket_queue& queue, Database& database, const std::shared_ptr<Tcp_server>& server);

private:
	virtual void run();
	void handle_connection();
	void recieve_data_from_socket();

	std::shared_ptr<Tcp_server> server_;
	Socket_queue& queue_;
	Database& database_;
	Socket socket_;
};


#endif
