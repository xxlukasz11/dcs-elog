#ifndef _CONNECTION_HANDLER_H_
#define _CONNECTION_HANDLER_H_

#include <mutex>
#include "msg_parser.h"
#include "socket.h"

class Connection_handler {
	Msg_parser& parser_;
	Socket socket_;
	static std::mutex mtx_;

public:
	Connection_handler(Msg_parser& parser, Socket socket);
	void handle();

private:
	template<Msg_parser::mode mode>
	void handle() = delete;

};

#endif