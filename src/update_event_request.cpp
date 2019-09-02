#include "update_event_request.h"

Update_event_request::Update_event_request() : Message(Type::update_event) {
}

void Update_event_request::extract_parameters(Msg_parser& parser) {
	event_id_ = parser.next();
	title_ = parser.next();
	description_ = parser.next();
	tags_ = parser.next();
}

std::string Update_event_request::name() const {
	return "UPDATE_EVENT_REQUEST";
}

const std::string& Update_event_request::get_event_id() const {
	return event_id_;
}

const std::string& Update_event_request::get_title() const {
	return title_;
}

const std::string& Update_event_request::get_description() const {
	return description_;
}

const std::string& Update_event_request::get_tags() const {
	return tags_;
}
