#include "create_event_request.h"
#include "utils.h"

void Create_event_request::extract_parameters(Msg_parser& parser) {
	title_ = parser.next();
	description_ = parser.next();
	tags_ = parser.next();
	author_ = parser.next();

	std::string attachachments_string = parser.next();
	utils::Pair_array pairs = utils::string_to_pair_array(';', attachachments_string);
	attachments_info_ = utils::pair_array_to_attachment(pairs);
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

const std::vector<Attachment_info>& Create_event_request::get_attachments_info() const {
	return attachments_info_;
}
