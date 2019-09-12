#include "return_events_request.h"

Return_events_request::Return_events_request() : Message(Type::return_events) {
}

void Return_events_request::extract_parameters(Msg_parser& parser) {
	min_date_ = parser.next();
	max_date_ = parser.next();
	tags_ = parser.next();
}

std::string Return_events_request::name() const {
	return "RETURN_EVENTS_REQUEST";
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
