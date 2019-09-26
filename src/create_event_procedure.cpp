#include "prepared_statement.h"
#include "result_set.h"
#include "utils.h"
#include "create_event_procedure.h"

Create_event_procedure::Create_event_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Create_event_request>(message)) {
}

void Create_event_procedure::start() {
	Insert_query query = prepare_query();
	std::string response_message = run_main_procedure(query);
	socket_.send_string(response_message);
}

std::string Create_event_procedure::name() {
	return "CREATE_EVENT_PROCEDURE";
}

Insert_query Create_event_procedure::prepare_query() const {
	Insert_query query;
	query.set_title(message_->get_title());
	query.set_desc(message_->get_description());
	query.set_tags(utils::string_to_vector(message_->get_tags()));
	query.set_author(message_->get_author());
	return query;
}

std::string Create_event_procedure::run_main_procedure(const Insert_query & query) {
	Prepared_statement events_stmt = query.create_events_statement();
	Prepared_statement exists_stmt = query.create_tags_exist_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	auto not_existing_tags = load_not_existing_tags(exists_stmt);
	if (not_existing_tags.empty()) {
		Database::Transaction transaction(database_);
		Result_set res = database_.execute(events_stmt);
		std::string last_id = res.get_last_row_id();
		database_.execute(query.create_tags_statement(last_id));
		transaction.commit();
		return "Event with id: " + last_id + " has been successfully saved";
	}
	else {
		accessor.close();
		std::string message = "Cannot save event. Following tags do not exist: ";
		message += utils::concatenate_string_array(not_existing_tags);
		return message;
	}
}

std::vector<std::string> Create_event_procedure::load_not_existing_tags(const Prepared_statement& stmt) {
	Result_set not_existing_tags = database_.execute(stmt);
	return not_existing_tags.get_column(0);
}
