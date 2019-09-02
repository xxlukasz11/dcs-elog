#include <memory>
#include "message.h"
#include "utils.h"

Message::Message(Message::Type message_type) : message_type_(message_type) {
}

Message::Type Message::get_message_type() const {
	return message_type_;
}

Message::Type Message::int_to_message_type(int mode) {
	return static_cast<Message::Type>(mode);
}
