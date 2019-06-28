#include "delete_tag_query.h"

namespace {
	namespace sql {
		std::string select_tag_name = "SELECT tag FROM Tags_list WHERE id = ?;";
		std::string get_parent_id_or_default = "SELECT CASE WHEN (sub.parent_id IS NULL OR sub.parent_id = '') THEN (SELECT id FROM Tags_list WHERE Tag = '-') ELSE (SELECT parent_id FROM Tags_tree WHERE id = ?) END AS parent_id FROM (SELECT parent_id FROM Tags_tree WHERE id = ?) sub;";
		std::string get_parent_id_or_null = "SELECT parent_id FROM Tags_tree WHERE id = ?";
		
		std::string delete_events_tag = "DELETE FROM tags_of_events WHERE event_id IN (SELECT event_id FROM tags_of_events WHERE tag_id = ?) AND tag_id = ?;";
		std::string update_events_tag = "UPDATE Tags_of_events SET tag_id = ? WHERE tag_id = ?;";
		std::string delete_redundant_records = "DELETE FROM Tags_of_events WHERE event_id IN (SELECT event_id FROM (SELECT event_id, count(event_id) AS num_of_tags FROM tags_of_events WHERE event_id IN(SELECT event_id FROM Tags_of_events WHERE tag_id = (SELECT id FROM Tags_list WHERE tag = '-')) GROUP BY event_id) where num_of_tags > 1) AND tag_id = (SELECT id FROM Tags_list WHERE tag = '-');";

		std::string update_tree = "UPDATE tags_tree SET parent_id = ? WHERE parent_id = ?;";
		std::string delete_from_tree = "DELETE from Tags_tree WHERE id = ?";
		std::string delete_from_list = "DELETE FROM Tags_list WHERE id = ?";
	}
}

void Delete_tag_query::set_tag_id(const std::string& tag_id) {
	tag_id_ = tag_id;
}

const std::string& Delete_tag_query::get_tag_id() const {
	return tag_id_;
}

Prepared_statement Delete_tag_query::select_statement() const {
	Prepared_statement stmt{ sql::select_tag_name };
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::parent_id_statement(value mode) const {
	if (mode == ALLOW_NULL) {
		Prepared_statement stmt{ sql::get_parent_id_or_null };
		stmt.add_param(tag_id_);
		return stmt;
	}
	else {
		Prepared_statement stmt{ sql::get_parent_id_or_default };
		stmt.add_param(tag_id_);
		stmt.add_param(tag_id_);
		return stmt;
	}
}

Prepared_statement Delete_tag_query::delete_events_tag_statement(const std::string& parent_id) const {
	Prepared_statement stmt{ sql::delete_events_tag };
	stmt.add_param(tag_id_);
	stmt.add_param(parent_id);
	return stmt;
}

Prepared_statement Delete_tag_query::update_events_tag_statement(const std::string& parent_id) const {
	Prepared_statement stmt{ sql::update_events_tag };
	stmt.add_param(parent_id);
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::delete_redundant_statement() const {
	Prepared_statement stmt{ sql::delete_redundant_records };
	return stmt;
}

Prepared_statement Delete_tag_query::update_tree_statement(const std::string& parent_id) const {
	Prepared_statement stmt{ sql::update_tree };
	stmt.add_param(parent_id);
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::delete_tree_statement() const {
	Prepared_statement stmt{ sql::delete_from_tree };
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::delete_list_statement() const {
	Prepared_statement stmt{ sql::delete_from_list };
	stmt.add_param(tag_id_);
	return stmt;
}
