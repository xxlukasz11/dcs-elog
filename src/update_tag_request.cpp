#include "update_tag_request.h"

Update_tag_request::Update_tag_request() : Message(Type::update_tag) {
}

void Update_tag_request::extract_parameters(Msg_parser& parser) {
	tag_id_ = parser.next();
	new_name_ = parser.next();
}

std::string Update_tag_request::name() const {
	return "UPDATE_TAG_REQUEST";
}

const std::string& Update_tag_request::get_tag_id() const {
	return tag_id_;
}

const std::string& Update_tag_request::get_new_name() const {
	return new_name_;
}
