#include <memory>
#include "message.h"

Message::Type Message::int_to_message_type(int mode) {
	return static_cast<Message::Type>(mode);
}
