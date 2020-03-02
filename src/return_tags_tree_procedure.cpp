#include <utility>
#include "select_tags_query.h"
#include "json_stringifier.h"
#include "prepared_statement.h"
#include "return_tags_tree_procedure.h"

static const std::string ON_SUCCESS_MESSAGE = "Tags tree has been loaded";

void Return_tags_tree_procedure::start() {
	const Select_tags_query query;
	const auto stmt = query.create_sql();
	Result_set tags_tree = load_tags_tree(stmt);

	response_.set_success(ON_SUCCESS_MESSAGE);
	response_.set_data(std::move(tags_tree));
	log_response_message(response_);
	send_response(std::move(response_));
}

std::string Return_tags_tree_procedure::name() {
	return "RETURN_TAGS_TREE_PROCEDURE";
}

void Return_tags_tree_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Return_tags_tree_request>(message);
}

Result_set Return_tags_tree_procedure::load_tags_tree(const Prepared_statement& stmt) {
	Database::Accessor accessor(database_);
	accessor.open();
	Result_set tags_tree = database_.execute(stmt);
	return tags_tree;
}
