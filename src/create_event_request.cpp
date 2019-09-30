#include "create_event_request.h"

void Create_event_request::extract_parameters(Msg_parser& parser) {
	title_ = parser.next();
	description_ = parser.next();
	tags_ = parser.next();
	author_ = parser.next();
}

std::string Create_event_request::name() const {
	return "CREATE_EVENT_REQUEST";
}

Message::Type Create_event_request::get_message_type() const {
	return Message::Type::create_event;
}

const std::string& Create_event_request::get_title() const {
	return title_;
}

const std::string& Create_event_request::get_description() const {
	return description_;
}

const std::string& Create_event_request::get_tags() const {
	return tags_;
}

const std::string& Create_event_request::get_author() const {
	return author_;
}
