#include <utility>
#include <string>
#include <vector>

#include "custom_exceptions.h"
#include "insert_query.h"
#include "config.h"

namespace {
	namespace sql {
		std::string tags_exist_before = "SELECT input.tag FROM (SELECT ? AS tag";
		std::string tags_exist_after = ") input WHERE input.tag NOT IN (SELECT tag FROM Tags_list);";
		std::string tags_exist_union = " UNION SELECT ?";

		std::string insert_event = "INSERT INTO Events VALUES(null, strftime('%s'), ?, ?, ?);";
		std::string insert_tags_before = "INSERT INTO Tags_of_events(Event_id, Tag_id) SELECT ?, Tags_list.id FROM Tags_list WHERE ";
		std::string insert_tags_condition = "Tags_list.tag = ?";
	}
}

void Insert_query::set_title(std::string title) {
	title_ = title;
	title_is_set_ = true;
}

void Insert_query::set_desc(std::string desc) {
	desc_ = desc;
	desc_is_set_ = true;
}

void Insert_query::create_tag(std::string tag) {
	tags_.push_back(tag);
}

void Insert_query::set_tags(std::vector<std::string>&& tags){
	tags_ = std::move(tags);
	if (tags_.empty()) {
		tags_.push_back(config::database::empty_tag_name);
	}
}

void Insert_query::set_author(const std::string& author) {
	author_ = author;
	author_is_set_ = true;
}

const std::vector<std::string>& Insert_query::get_tags() const {
	return tags_;
}

Prepared_statement Insert_query::create_tags_exist_statement() const {
	size_t n_of_tags = tags_.size();
	if (n_of_tags == 0)
		return Prepared_statement();

	Prepared_statement stmt{ sql::tags_exist_before };
	stmt.add_param(tags_[0]);

	for (size_t i = 1; i < n_of_tags; ++i) {
		stmt += sql::tags_exist_union;
		stmt.add_param(tags_[i]);
	}

	stmt += sql::tags_exist_after;
	return stmt;
}

Prepared_statement Insert_query::create_events_statement() const {
	if(!title_is_set_ && !desc_is_set_){
		throw Query_error("Insert query for Events table is empty");
	}

	Prepared_statement p_stmt{ sql::insert_event };
	p_stmt.add_param(title_);
	p_stmt.add_param(desc_);
	p_stmt.add_param(author_);
	return p_stmt;
}

Prepared_statement Insert_query::create_tags_statement(const std::string& event_id) const {
	Prepared_statement stmt{ sql::insert_tags_before };
	stmt.add_param(event_id);

	size_t n_tags = tags_.size();
	for (size_t i = 0; i < n_tags; ++i) {
		if (i != 0)
			stmt += " OR ";
		stmt += sql::insert_tags_condition;
		stmt.add_param(tags_[i]);
	}
	stmt += ";";
	return stmt;
}
