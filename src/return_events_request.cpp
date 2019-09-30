#include "return_events_request.h"

void Return_events_request::extract_parameters(Msg_parser& parser) {
	min_date_ = parser.next();
	max_date_ = parser.next();
	tags_ = parser.next();
}

std::string Return_events_request::name() const {
	return "RETURN_EVENTS_REQUEST";
}

Message::Type Return_events_request::get_message_type() const {
	return Message::Type::return_events;
}

const std::string& Return_events_request::get_min_date() const {
	return min_date_;
}

const std::string& Return_events_request::get_max_date() const {
	return max_date_;
}

const std::string& Return_events_request::get_tags() const {
	return tags_;
}
