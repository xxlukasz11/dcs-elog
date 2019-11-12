#include <utility>
#include "add_tag_query.h"
#include "result_set.h"
#include "prepared_statement.h"
#include "create_tag_procedure.h"

void Create_tag_procedure::start() {
	Add_tag_query query = prepare_query();
	create_tag(query);
	send_response(std::move(response_));
}

std::string Create_tag_procedure::name() {
	return "CREATE_TAG_PROCEDURE";
}

void Create_tag_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Create_tag_request>(message);
}

Add_tag_query Create_tag_procedure::prepare_query() const {
	Add_tag_query query;
	query.set_tag_name(message_->get_tag_name());
	query.set_parent_id(message_->get_parent_id());
	return query;
}

void Create_tag_procedure::create_tag(const Add_tag_query& query) {
	Prepared_statement tag_exists_statement = query.create_get_tag_id_statement();
	Prepared_statement add_tag_statement = query.create_tag_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	Result_set tag_exists_result = database_.execute(tag_exists_statement);
	if (tag_exists_result.has_records()) {
		response_.set_failure("Tag '" + query.get_tag_name() + "' already exists");
	}
	else {
		Database::Transaction transaction(database_);
		Result_set res = database_.execute(add_tag_statement);
		std::string last_id = res.get_last_row_id();
		database_.execute(query.create_tree_statement(last_id));
		transaction.commit();
		response_.set_success("Tag '" + query.get_tag_name() + "' has been created");
	}
}
