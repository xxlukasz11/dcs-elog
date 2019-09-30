#include "return_tags_tree_request.h"

void Return_tags_tree_request::extract_parameters(Msg_parser& parser) {

}

std::string Return_tags_tree_request::name() const {
	return "RETURN_TAGS_TREE_REQUEST";
}

Message::Type Return_tags_tree_request::get_message_type() const {
	return Message::Type::return_tags_tree;
}
