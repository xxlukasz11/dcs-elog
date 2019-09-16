#include "utils.h"
#include "prepared_statement.h"
#include "result_set.h"
#include "update_event_procedure.h"

Update_event_procedure::Update_event_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Update_event_request>(message)) {
}

void Update_event_procedure::start() {
	Update_event_query update_query = prepare_update_query();
	Insert_query insert_query = prepare_insert_query();
	std::string response_message = run_main_procedure(update_query, insert_query);
	socket_.send_string(response_message);
}

Update_event_query Update_event_procedure::prepare_update_query() const {
	Update_event_query query;
	query.set_id(message_->get_event_id());
	query.set_title(message_->get_title());
	query.set_tags(utils::string_to_vector(message_->get_tags()));
	query.set_description(message_->get_description());
	return query;
}

Insert_query Update_event_procedure::prepare_insert_query() const {
	Insert_query query;
	query.set_tags(utils::string_to_vector(message_->get_tags()));
	return query;
}

std::string Update_event_procedure::run_main_procedure(const Update_event_query& update_query, const Insert_query& insert_query) {
	Prepared_statement event_exists_stmt = update_query.create_event_exists_stmt();
	Prepared_statement tags_exists_stmt = insert_query.create_tags_exist_statement();
	Prepared_statement update_event_stmt = update_query.create_update_event_stmt();
	Prepared_statement delete_event_tags_stmt = update_query.create_delete_event_tags_statement();
	Prepared_statement insert_event_tags_stmt = insert_query.create_tags_statement(message_->get_event_id());

	Database::Accessor accessor(database_);
	accessor.open();

	if (!check_if_event_exists(event_exists_stmt)) {
		return "Cannot update event with id: " + message_->get_event_id() + ". It does not exist";
	}

	auto not_existing_tags = load_not_existing_tags(tags_exists_stmt);
	if (!not_existing_tags.empty()) {
		std::string message = "Cannot update event. Following tags do not exist: ";
		message += utils::concatenate_string_array(not_existing_tags);
		return message;
	}

	database_.execute(update_event_stmt);
	database_.execute(delete_event_tags_stmt);
	if (update_query.has_any_tags()) {
		database_.execute(insert_event_tags_stmt);
	}

	return "Event with id: " + message_->get_event_id() + " has been successfully updated";
}

bool Update_event_procedure::check_if_event_exists(const Prepared_statement& stmt) {
	Result_set event_exists_result = database_.execute(stmt);
	bool has_event = event_exists_result.has_records();
	return has_event;
}

std::vector<std::string> Update_event_procedure::load_not_existing_tags(const Prepared_statement& stmt) {
	Result_set not_existing_tags = database_.execute(stmt);
	return not_existing_tags.get_column(0);
}
