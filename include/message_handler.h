#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include <memory>
#include "message.h"
#include "socket.h"
#include "database.h"
#include "procedure.h"
#include "attachment_handler.h"

class Message_handler {
public:
	Message_handler(Socket socket, Database& database);
	void process_message(const std::string& message);

private:
	void handle(const std::shared_ptr<Message>& message_string);
	std::unique_ptr<Procedure> create_procedure(const std::shared_ptr<Message>& message);

	template<typename T>
	std::unique_ptr<Procedure> create_procedure(const std::shared_ptr<Message>& message);

	Socket socket_;
	Database& database_;
	Attachment_handler attachment_handler_;
};

template<typename T>
inline std::unique_ptr<Procedure> Message_handler::create_procedure(const std::shared_ptr<Message>& message) {
	auto procedure = std::make_unique<T>(database_, socket_, attachment_handler_);
	procedure->set_message(message);
	return procedure;
}

#endif
