#include "utils.h"
#include "json.h"
#include "prepared_statement.h"
#include "return_events_procedure.h"

Return_events_procedure::Return_events_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Return_events_request>(message)) {
}

void Return_events_procedure::start() {
	auto query = prepare_query();
	auto stmt = query.create_statement();
	Result_set events = load_events(stmt);
	socket_.send_string(
		json::stringify(
			std::move(events)));
}

std::string Return_events_procedure::name() {
	return "RETURN_EVENTS_PROCEDURE";
}

Select_query Return_events_procedure::prepare_query() const {
	Select_query query;
	query.set_min_date(message_->get_min_date());
	query.set_max_date(message_->get_max_date());
	auto extracted_tags = utils::string_to_vector(message_->get_tags());
	query.set_tags(std::move(extracted_tags));
	return query;
}

Result_set Return_events_procedure::load_events(const Prepared_statement& stmt) {
	Database::Accessor accessor(database_);
	accessor.open();
	Result_set events = database_.execute(stmt);
	return events;
}
