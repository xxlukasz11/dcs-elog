#include "create_event_request.h"
#include "utils.h"

void Create_event_request::extract_parameters(Msg_parser& parser) {
	title_ = parser.next();
	description_ = parser.next();
	tags_ = parser.next();
	author_ = parser.next();

	std::string attachachments_string = parser.next();
	attachments_names_ = utils::string_to_vector(attachachments_string);
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

const std::vector<std::string>& Create_event_request::get_attachments_names() const {
	return attachments_names_;
}
