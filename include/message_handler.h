#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include <memory>
#include "message.h"
#include "socket.h"
#include "database.h"
#include "return_events_request.h"
#include "return_tags_tree_request.h"
#include "create_event_request.h"
#include "create_tag_request.h"
#include "delete_tag_request.h"
#include "update_event_request.h"
#include "update_tag_request.h"

class Message_handler {
	Socket socket_;
	Database& database_;

public:
	Message_handler(Socket socket, Database& database);
	void handle(const std::shared_ptr<Message>& message);

private:
	void handle_message(const Create_event_request& message);
	void handle_message(const Create_tag_request& message);
	void handle_message(const Delete_tag_request& message);
	void handle_message(const Return_events_request& message);
	void handle_message(const Return_tags_tree_request& message);
	void handle_message(const Update_event_request& message);
	void handle_message(const Update_tag_request& message);

};

#endif