#include <utility>
#include "select_tags_query.h"
#include "json_stringifier.h"
#include "prepared_statement.h"
#include "return_tags_tree_procedure.h"

static const std::string ON_SUCCESS_MESSAGE = "Tags tree has been loaded";

Return_tags_tree_procedure::Return_tags_tree_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Return_tags_tree_request>(message)) {
}

void Return_tags_tree_procedure::start() {
	Select_tags_query query;
	auto stmt = query.create_sql();
	Result_set tags_tree = load_tags_tree(stmt);

	response_.set_success(ON_SUCCESS_MESSAGE);
	response_.set_data(tags_tree);
	send_response(std::move(response_));
}

std::string Return_tags_tree_procedure::name() {
	return "RETURN_TAGS_TREE_PROCEDURE";
}

Result_set Return_tags_tree_procedure::load_tags_tree(const Prepared_statement& stmt) {
	Database::Accessor accessor(database_);
	accessor.open();
	Result_set tags_tree = database_.execute(stmt);
	return tags_tree;
}
