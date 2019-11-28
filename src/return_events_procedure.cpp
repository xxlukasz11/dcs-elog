#include <utility>
#include "utils.h"
#include "json_stringifier.h"
#include "prepared_statement.h"
#include "return_events_procedure.h"

static const std::string ON_SUCCESS_MESSAGE = "Successfully loaded events";

void Return_events_procedure::start() {
	auto query = prepare_query();
	auto stmt = query.create_statement();
	Result_set events = load_events(stmt);

	response_.set_success(ON_SUCCESS_MESSAGE);
	response_.set_data(std::move(events));
	send_response(std::move(response_));
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
