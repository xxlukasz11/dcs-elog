#include <utility>
#include "prepared_statement.h"
#include "result_set.h"
#include "utils.h"
#include "json_stringifier.h"
#include "create_event_procedure.h"

void Create_event_procedure::start() {
	attachment_handler_rx_.handle_attachments(
		message_->get_attachments_info());
	Insert_query query = prepare_query();
	run_main_procedure(query);
	log_response_message(response_);
	send_response(std::move(response_));
}

std::string Create_event_procedure::name() {
	return "CREATE_EVENT_PROCEDURE";
}

void Create_event_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Create_event_request>(message);
}

Insert_query Create_event_procedure::prepare_query() const {
	Insert_query query;
	query.set_title(message_->get_title());
	query.set_desc(message_->get_description());
	query.set_tags(utils::string_to_vector(message_->get_tags()));
	query.set_author(message_->get_author());
	query.set_attachments(attachment_handler_rx_.get_attachment_array());
	return query;
}

void Create_event_procedure::run_main_procedure(Insert_query & query) {
	const Prepared_statement events_stmt = query.create_events_statement();
	const Prepared_statement exists_stmt = query.create_tags_exist_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	const auto not_existing_tags = load_not_existing_tags(exists_stmt);
	if (not_existing_tags.empty()) {
		Database::Transaction transaction(database_);
		const Result_set res = database_.execute(events_stmt);
		const std::string last_id = res.get_last_row_id();
		database_.execute(query.create_tags_statement(last_id));
		if (query.has_attachments()) {
			database_.execute(query.create_attachments_statement(last_id));
		}
		transaction.commit();
		attachment_handler_rx_.commit();
		yeld_success(last_id);
	}
	else {
		accessor.close();
		yeld_missing_tags_error(not_existing_tags);
	}
}

void Create_event_procedure::yeld_success(const std::string& event_id) {
	const std::string message = "Event with id: " + event_id + " has been successfully saved";
	response_.set_success(message);
}

void Create_event_procedure::yeld_missing_tags_error(const std::vector<std::string>& not_existing_tags) {
	const std::string message = "Cannot save event. Following tags do not exist: "
		+ utils::concatenate_string_array(not_existing_tags);
	response_.set_failure(message);
}

std::vector<std::string> Create_event_procedure::load_not_existing_tags(const Prepared_statement& stmt) {
	const Result_set not_existing_tags = database_.execute(stmt);
	return not_existing_tags.get_column(0);
}
