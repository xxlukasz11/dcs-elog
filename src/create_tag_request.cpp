#include "create_tag_request.h"

void Create_tag_request::extract_parameters(Msg_parser& parser) {
	tag_name_ = parser.next();
	parent_id_ = parser.next();
}

std::string Create_tag_request::name() const {
	return "CREATE_TAG_REQUEST";
}

Message::Type Create_tag_request::get_message_type() const {
	return Message::Type::create_tag;
}

const std::string& Create_tag_request::get_tag_name() const {
	return tag_name_;
}

const std::string& Create_tag_request::get_parent_id() const {
	return parent_id_;
}
