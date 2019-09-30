#include "utils.h"
#include "config.h"
#include "database.h"
#include "result_set.h"
#include <string>
#include "add_tag_query.h"
#include "create_library_event_procedure.h"

std::string Create_library_event_procedure::name() {
	return "CREATE_LIBRARY_EVENT_PROCEDURE";
}

void Create_library_event_procedure::leave_only_existing_tags(Insert_query& query,
	const std::vector<std::string>& not_existing_tags) {
	auto existing_tags = utils::exclude_from_array(query.get_tags(), not_existing_tags);
	existing_tags.push_back(config::database::empty_tag_name);
	query.set_tags(std::move(existing_tags));
}

std::string Create_library_event_procedure::run_main_procedure(Insert_query& query) {
	Prepared_statement events_stmt = query.create_events_statement();
	Prepared_statement exists_stmt = query.create_tags_exist_statement();

	Database::Accessor accessor(database_);
	accessor.open();

	auto not_existing_tags = load_not_existing_tags(exists_stmt);
	if (!not_existing_tags.empty()) {
		leave_only_existing_tags(query, not_existing_tags);
	}
	
	Database::Transaction transaction(database_);
	Result_set res = database_.execute(events_stmt);
	std::string last_id = res.get_last_row_id();
	database_.execute(query.create_tags_statement(last_id));
	transaction.commit();
	return "Event with id: " + last_id + " has been successfully saved";
}
