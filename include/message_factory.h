#ifndef _MESSAGE_FACTORY_H_
#define _MESSAGE_FACTORY_H_

#include <string>
#include <memory>
#include "message.h"

class Message_factory {
public:
	Message_factory(const std::string& message_string);
	std::shared_ptr<Message> create() const;

private:
	std::shared_ptr<Message> create_message(Message::Type message_type) const;

	std::string message_string_;
};

#endif