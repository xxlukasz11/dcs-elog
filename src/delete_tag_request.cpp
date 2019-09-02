#include "delete_tag_request.h"

Delete_tag_request::Delete_tag_request() : Message(Type::delete_tag) {
}

void Delete_tag_request::extract_parameters(Msg_parser& parser) {
	tag_id_ = parser.next();
}

std::string Delete_tag_request::name() const {
	return "DELETE_TAG_REQUEST";
}

const std::string& Delete_tag_request::get_tag_id() const {
	return tag_id_;
}
