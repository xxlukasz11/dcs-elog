#include "create_library_event_request.h"

std::string Create_library_event_request::name() const {
	return "CREATE_LIBRARY_EVENT_REQUEST";
}

Message::Type Create_library_event_request::get_message_type() const {
	return Message::Type::create_library_event;
}
