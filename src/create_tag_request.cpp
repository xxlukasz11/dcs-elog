#include "create_tag_request.h"

Create_tag_request::Create_tag_request() : Message(Type::create_tag) {
}

void Create_tag_request::extract_parameters(Msg_parser& parser) {
	tag_name_ = parser.next();
	parent_id_ = parser.next();
}

std::string Create_tag_request::name() const {
	return "CREATE_TAG_REQUEST";
}

const std::string& Create_tag_request::get_tag_name() const {
	return tag_name_;
}

const std::string& Create_tag_request::get_parent_id() const {
	return parent_id_;
}
