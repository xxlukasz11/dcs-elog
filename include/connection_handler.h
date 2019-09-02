#ifndef _CONNECTION_HANDLER_H_
#define _CONNECTION_HANDLER_H_

#include <mutex>
#include <memory>
#include "message.h"
#include "socket.h"

#include "return_events_request.h"
#include "return_tags_tree_request.h"
#include "create_event_request.h"
#include "create_tag_request.h"
#include "delete_tag_request.h"
#include "update_event_request.h"
#include "update_tag_request.h"

class Connection_handler {
	Socket socket_;
	static std::mutex mtx_;

public:
	Connection_handler(Socket socket);
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