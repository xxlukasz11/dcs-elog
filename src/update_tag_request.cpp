#include "update_tag_request.h"

void Update_tag_request::extract_parameters(Msg_parser& parser) {
	tag_id_ = parser.next();
	new_name_ = parser.next();
}

std::string Update_tag_request::name() const {
	return "UPDATE_TAG_REQUEST";
}

Message::Type Update_tag_request::get_message_type() const {
	return Message::Type::update_tag;
}

const std::string& Update_tag_request::get_tag_id() const {
	return tag_id_;
}

const std::string& Update_tag_request::get_new_name() const {
	return new_name_;
}
