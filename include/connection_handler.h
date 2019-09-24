#ifndef _CONNECTION_HANDLER_H_
#define _CONNECTION_HANDLER_H_

#include "concurrent_queue.h"
#include "database.h"
#include "msg_parser.h"
#include "dcs_thread.h"
#include "socket.h"

class Tcp_server;

class Connection_handler : public Dcs_thread {
	std::shared_ptr<Tcp_server> server_;
	Socket_queue& queue_;
	Database& database_;
	Socket socket_;
public:
	Connection_handler(Socket_queue& queue, Database& database, const std::shared_ptr<Tcp_server>& server);
private:
	virtual void run();
	void handle_connection();
	void recieve_data_from_socket();
};


#endif
