#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include <memory>
#include "message.h"
#include "socket.h"
#include "database.h"
#include "procedure.h"

class Message_handler {
public:
	Message_handler(Socket socket, Database& database);
	void process_message(const std::string& message);

private:
	void handle(const std::shared_ptr<Message>& message_string);
	std::unique_ptr<Procedure> create_procedure(const std::shared_ptr<Message>& message);

	Socket socket_;
	Database& database_;
};

#endif