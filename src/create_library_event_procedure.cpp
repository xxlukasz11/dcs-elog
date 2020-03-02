#include <string>
#include "utils.h"
#include "administrator.h"
#include "database.h"
#include "logger.h"
#include "result_set.h"
#include "add_tag_query.h"
#include "json_stringifier.h"
#include "prepared_statement.h"
#include "create_library_event_procedure.h"

Insert_query Create_library_event_procedure::prepare_query() const {
	Insert_query query;
	query.set_title(message_->get_title());
	query.set_desc(message_->get_description());
	query.set_tags(utils::string_to_vector(message_->get_tags()));
	query.set_author(message_->get_author());
	return query;
}

void Create_library_event_procedure::start() {
	Insert_query query = prepare_query();
	try {
		run_main_procedure(query);
	} catch (...) {
		Logger::create().context(socket_).level(Log_level::CRITICAL).info("Failed to create library event");
		throw;
	}
	
	int32_t response_code = static_cast<int32_t>(response_.get_response_code());
	socket_.send_value(response_code);
}

std::string Create_library_event_procedure::name() {
	return "CREATE_LIBRARY_EVENT_PROCEDURE";
}

void Create_library_event_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Create_library_event_request>(message);
}

void Create_library_event_procedure::leave_only_existing_tags(Insert_query& query,
	const std::vector<std::string>& not_existing_tags) {
	auto existing_tags = utils::exclude_from_array(query.get_tags(), not_existing_tags);
	existing_tags.push_back(Administrator::instance().params().get_empty_tag_name());
	query.set_tags(std::move(existing_tags));
}

void Create_library_event_procedure::run_main_procedure(Insert_query& query) {
	const Prepared_statement events_stmt = query.create_events_statement();
	const Prepared_statement exists_stmt = query.create_tags_exist_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	const auto not_existing_tags = load_not_existing_tags(exists_stmt);
	if (!not_existing_tags.empty()) {
		leave_only_existing_tags(query, not_existing_tags);
	}
	
	Database::Transaction transaction(database_);
	const Result_set res = database_.execute(events_stmt);
	const std::string last_id = res.get_last_row_id();
	database_.execute(query.create_tags_statement(last_id));
	transaction.commit();
	response_.set_success();
	Logger::create().context(socket_).level(Log_level::INFO).info("Event with id: "
		+ last_id + " has been successfully saved");
}

std::vector<std::string> Create_library_event_procedure::load_not_existing_tags(const Prepared_statement& stmt) {
	const Result_set not_existing_tags = database_.execute(stmt);
	return not_existing_tags.get_column(0);
}
