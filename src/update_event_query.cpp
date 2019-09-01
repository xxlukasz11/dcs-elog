#include <utility>
#include "update_event_query.h"
#include "custom_exceptions.h"

namespace {
	namespace sql {
		std::string event_exists = "SELECT id FROM Events WHERE id = ?;";
		std::string update_event = "UPDATE Events SET Title = ?, Description = ? WHERE Id = ?;";
		std::string delete_event_tags = "DELETE FROM Tags_of_events WHERE Event_id = ?;";
	}
}

void Update_event_query::set_id(const std::string& id) {
	id_ = id;
	id_is_set_ = true;
}
void Update_event_query::set_title(const std::string& title) {
	title_ = title;
	title_is_set_ = true;
}

void Update_event_query::set_description(const std::string& description) {
	description_ = description;
	description__is_set_ = true;
}

void Update_event_query::set_tags(std::vector<std::string>&& tags) {
	tags_ = std::move(tags);
}

bool Update_event_query::has_any_tags() const {
	return tags_.size() > 0;
}

Prepared_statement Update_event_query::create_event_exists_stmt() const {
	if (!id_is_set_) {
		throw Query_error("Update event query error. Id is not set");
	}
	Prepared_statement stmt{ sql::event_exists };
	stmt.add_param(id_);
	return stmt;
}

Prepared_statement Update_event_query::create_update_event_stmt() const {
	if (!id_is_set_ || !title_is_set_ || !description__is_set_) {
		throw Query_error("Update event query error. One of the parameters is not set");
	}
	Prepared_statement stmt{ sql::update_event };
	stmt.add_param(title_);
	stmt.add_param(description_);
	stmt.add_param(id_);
	return stmt;
}
Prepared_statement Update_event_query::create_delete_event_tags_statement() const {
	if (!id_is_set_) {
		throw Query_error("Update event query error. Id is not set");
	}
	Prepared_statement stmt{ sql::delete_event_tags };
	stmt.add_param(id_);
	return stmt;
}