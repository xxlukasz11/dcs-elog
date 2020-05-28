#ifndef _MESSAGE_FACTORY_H_
#define _MESSAGE_FACTORY_H_

#include <string>
#include <memory>
#include "message.h"

/*
Creates message basing on message type
*/
class Message_factory {
public:
	Message_factory(const std::string& message_string);
	std::shared_ptr<Message> create() const;

private:
	Message::Type extract_message_type(Msg_parser& parser) const;
	std::shared_ptr<Message> create_message(Message::Type message_type) const;
	void read_message_contents(const std::shared_ptr<Message>& message, Msg_parser& parser) const;

	std::string message_string_;
};

#endif