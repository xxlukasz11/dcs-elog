#include <utility>
#include "administrator.h"
#include "result_set.h"
#include "prepared_statement.h"
#include "delete_tag_procedure.h"

void Delete_tag_procedure::start() {
	const Delete_tag_query query = prepare_query();
	run_main_procedure(query);
	log_response_message(response_);
	send_response(std::move(response_));
}

std::string Delete_tag_procedure::name() {
	return "DELETE_TAG_PROCEDURE";
}

void Delete_tag_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Delete_tag_request>(message);
}

Delete_tag_query Delete_tag_procedure::prepare_query() const {
	Delete_tag_query query;
	query.set_tag_id(message_->get_tag_id());
	return query;
}

void Delete_tag_procedure::run_main_procedure(const Delete_tag_query& query) {
	const Prepared_statement select_stmt = query.select_statement();
	const Prepared_statement parent_id_null_stmt = query.parent_id_statement(Delete_tag_query::ALLOW_NULL);
	const Prepared_statement parent_id_default_stmt = query.parent_id_statement(Delete_tag_query::RETURN_DEFAULT);
	const Prepared_statement delete_redundant_stmt = query.delete_redundant_statement();
	const Prepared_statement delete_tree_stmt = query.delete_tree_statement();
	const Prepared_statement delete_list_stmt = query.delete_list_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	const Result_set select_tag_result = database_.execute(select_stmt);
	bool tag_exists = check_if_tag_exists(select_tag_result);
	if (!tag_exists) {
		response_.set_failure("Tag with id: " + query.get_tag_id() + " does not exist");
		return;
	}
	const std::string tag_name = extract_tag_name(select_tag_result);
	bool tag_is_reserved = check_if_tag_is_reserved(tag_name);
	if (tag_is_reserved) {
		response_.set_failure("Tag '" + tag_name + "' cannot be deleted");
		return;
	}

	const Result_set res = database_.execute(parent_id_null_stmt);
	const std::string parent_id = res.get_first_field();

	const Result_set res_events = database_.execute(parent_id_default_stmt);
	const std::string parent_id_for_events = res_events.get_first_field();

	const Prepared_statement delete_events_tag_stmt = query.delete_events_tag_statement(parent_id_for_events);
	const Prepared_statement update_events_tag_stmt = query.update_events_tag_statement(parent_id_for_events);
	const Prepared_statement update_tree_stmt = query.update_tree_statement(parent_id);

	Database::Transaction transaction(database_);
	database_.execute(delete_events_tag_stmt);
	database_.execute(update_events_tag_stmt);
	database_.execute(delete_redundant_stmt);
	database_.execute(update_tree_stmt);
	database_.execute(delete_tree_stmt);
	database_.execute(delete_list_stmt);
	transaction.commit();

	response_.set_success("Tag '" + tag_name + "' has been deleted");
}

bool Delete_tag_procedure::check_if_tag_exists(const Result_set& result_set) {
	return result_set.has_records();
}

std::string Delete_tag_procedure::extract_tag_name(const Result_set& result_set) {
	return result_set.get_first_field();
}

bool Delete_tag_procedure::check_if_tag_is_reserved(const std::string tag_name) {
	return tag_name == Administrator::instance().params().get_empty_tag_name();
}
