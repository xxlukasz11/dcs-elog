#include "create_event_request.h"

Create_event_request::Create_event_request() : Message(Type::create_event) {
}

void Create_event_request::extract_parameters(Msg_parser& parser) {
	title_ = parser.next();
	description_ = parser.next();
	tags_ = parser.next();
	author_ = parser.next();
}

std::string Create_event_request::name() const {
	return "CREATE_EVENT_REQUEST";
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
