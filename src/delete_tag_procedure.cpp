#include <queue>
#include "config.h"
#include "result_set.h"
#include "prepared_statement.h"
#include "delete_tag_procedure.h"

Delete_tag_procedure::Delete_tag_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Delete_tag_request>(message)) {
}

void Delete_tag_procedure::start() {
	Delete_tag_query query = prepare_query();
	std::string response_message = run_main_procedure(query);
	socket_.send_string(response_message);
}

std::string Delete_tag_procedure::name() {
	return "DELETE_TAG_PROCEDURE";
}

Delete_tag_query Delete_tag_procedure::prepare_query() const {
	Delete_tag_query query;
	query.set_tag_id(message_->get_tag_id());
	return query;
}

std::string Delete_tag_procedure::run_main_procedure(const Delete_tag_query& query) {
	Prepared_statement select_stmt = query.select_statement();
	std::queue<Prepared_statement> statement_queue;
	Prepared_statement parent_id_null_stmt = query.parent_id_statement(Delete_tag_query::ALLOW_NULL);
	Prepared_statement parent_id_default_stmt = query.parent_id_statement(Delete_tag_query::RETURN_DEFAULT);
	Prepared_statement delete_redundant_stmt = query.delete_redundant_statement();
	Prepared_statement delete_tree_stmt = query.delete_tree_statement();
	Prepared_statement delete_list_stmt = query.delete_list_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	Result_set select_tag_result = database_.execute(select_stmt);
	bool tag_exists = check_if_tag_exists(select_tag_result);
	if (!tag_exists) {
		return "Tag with id: " + query.get_tag_id() + " does not exist";
	}
	std::string tag_name = extract_tag_name(select_tag_result);
	bool tag_is_reserved = check_if_tag_is_reserved(tag_name);
	if (tag_is_reserved) {
		return "Tag '" + tag_name + "' cannot be deleted";
	}

	Result_set res = database_.execute(parent_id_null_stmt);
	std::string parent_id = res.get_data()[0][0];

	Result_set res_events = database_.execute(parent_id_default_stmt);
	std::string parent_id_for_events = res_events.get_data()[0][0];

	Prepared_statement delete_events_tag_stmt = query.delete_events_tag_statement(parent_id_for_events);
	Prepared_statement update_events_tag_stmt = query.update_events_tag_statement(parent_id_for_events);
	Prepared_statement update_tree_stmt = query.update_tree_statement(parent_id);

	database_.execute(delete_events_tag_stmt);
	database_.execute(update_events_tag_stmt);
	database_.execute(delete_redundant_stmt);
	database_.execute(update_tree_stmt);
	database_.execute(delete_tree_stmt);
	database_.execute(delete_list_stmt);

	return "Tag '" + tag_name + "' has been deleted";
}

bool Delete_tag_procedure::check_if_tag_exists(const Result_set& result_set) {
	return result_set.has_records();
}

std::string Delete_tag_procedure::extract_tag_name(const Result_set& result_set) {
	return result_set.get_data()[0][0];
}

bool Delete_tag_procedure::check_if_tag_is_reserved(const std::string tag_name) {
	return tag_name == config::database::empty_tag_name;
}
