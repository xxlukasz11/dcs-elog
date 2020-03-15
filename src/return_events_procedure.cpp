#include <utility>
#include "utils.h"
#include "json_stringifier.h"
#include "prepared_statement.h"
#include "return_events_procedure.h"
#include "attachment_handler_tx.h"

static const std::string ON_SUCCESS_MESSAGE = "Successfully loaded events";

void Return_events_procedure::start() {
	const auto query = prepare_query();
	run_main_procedure(query);
	attachment_handler_tx_.inject_payload_and_send();
}

std::string Return_events_procedure::name() {
	return "RETURN_EVENTS_PROCEDURE";
}

void Return_events_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Return_events_request>(message);
}

Select_query Return_events_procedure::prepare_query() const {
	Select_query query;
	query.set_min_date(message_->get_min_date());
	query.set_max_date(message_->get_max_date());
	query.set_limit(message_->get_limit());
	query.set_offset(message_->get_offset());
	auto extracted_tags = utils::string_to_vector(message_->get_tags());
	query.set_tags(std::move(extracted_tags));
	return query;
}

Result_set Return_events_procedure::load_attachments_info(const Select_query& query, const Result_set& events) {
	const auto event_ids = events.get_column(0);
	Prepared_statement attachment_stmt = query.create_attachments_statement(event_ids);
	const Result_set attachments = database_.execute(attachment_stmt);
	return attachments;
}

void Return_events_procedure::run_main_procedure(const Select_query& query) {
	Database::Accessor accessor(database_);
	accessor.open();
	Result_set events = load_events(query);
	Result_set attachments = load_attachments_info(query, events);
	accessor.close();

	attachment_handler_tx_.set_events(std::move(events));
	attachment_handler_tx_.set_attachments(std::move(attachments));
	attachment_handler_tx_.set_on_success_message(ON_SUCCESS_MESSAGE);
}

Result_set Return_events_procedure::load_events(const Select_query& query) {
	const auto stmt = query.create_statement();
	const Result_set events = database_.execute(stmt);
	return events;
}
