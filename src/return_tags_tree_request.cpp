#include "return_tags_tree_request.h"

Return_tags_tree_request::Return_tags_tree_request() : Message(Type::return_tags_tree) {
}

void Return_tags_tree_request::extract_parameters(Msg_parser& parser) {

}

std::string Return_tags_tree_request::name() const {
	return "RETURN_TAGS_TREE_REQUEST";
}
