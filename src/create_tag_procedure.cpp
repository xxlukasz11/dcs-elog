#include "add_tag_query.h"
#include "result_set.h"
#include "prepared_statement.h"
#include "create_tag_procedure.h"

Create_tag_procedure::Create_tag_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Create_tag_request>(message)) {
}

void Create_tag_procedure::start() {
	Add_tag_query query = prepare_query();
	std::string response_message = create_tag(query);
	socket_.send_string(response_message);
}

std::string Create_tag_procedure::name() {
	return "CREATE_TAG_PROCEDURE";
}

Add_tag_query Create_tag_procedure::prepare_query() const {
	Add_tag_query query;
	query.set_tag_name(message_->get_tag_name());
	query.set_parent_id(message_->get_parent_id());
	return query;
}

std::string Create_tag_procedure::create_tag(const Add_tag_query& query) {
	Prepared_statement tag_exists_statement = query.create_get_tag_id_statement();
	Prepared_statement add_tag_statement = query.create_tag_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	Result_set tag_exists_result = database_.execute(tag_exists_statement);
	if (tag_exists_result.has_records()) {
		return "Tag '" + query.get_tag_name() + "' already exists";
	}
	else {
		Result_set res = database_.execute(add_tag_statement);
		std::string last_id = res.get_last_row_id();
		database_.execute(query.create_tree_statement(last_id));
		return "Tag '" + query.get_tag_name() + "' has been created";
	}
}
