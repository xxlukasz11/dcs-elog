#include <utility>
#include <string>
#include <vector>

#include "custom_exceptions.h"
#include "insert_query.h"

namespace {
	namespace sql {
		std::string tags_exist_before = "SELECT input.tag FROM (SELECT ? AS tag";
		std::string tags_exist_after = ") input WHERE input.tag NOT IN (SELECT tag FROM Tags_list);";
		std::string tags_exist_union = " UNION SELECT ?";
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

void Insert_query::add_tag(std::string tag) {
	tags_.push_back(tag);
}

void Insert_query::set_tags(std::vector<std::string>&& tags){
	tags_ = std::move(tags);
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

	Prepared_statement p_stmt{ "INSERT INTO Events VALUES(null, strftime('%s'), " };
	std::vector<std::string> params;

	if(title_is_set_){
		p_stmt += "?, ";
		p_stmt.add_param(title_);
	}
	else
		p_stmt += "null, ";

	if(desc_is_set_){
		p_stmt += "?, ";
		p_stmt.add_param(desc_);
	}
	else
		p_stmt += "null, ";

	if (author_is_set_) {
		p_stmt += "?";
		p_stmt.add_param(author_);
	}
	else
		p_stmt += "null";

	p_stmt += ");";
	return p_stmt;
}


std::vector<Prepared_statement> Insert_query::create_tags_statements(const std::string& event_id) const {
	std::vector<Prepared_statement> statements;

	for(const auto& tag : tags_){
		Prepared_statement p_stmt{ "INSERT INTO Tags_of_events(Event_id, Tag_id) SELECT ?, Tags_list.id FROM Tags_list WHERE Tags_list.tag = ?;" };
		p_stmt.add_param(event_id);
		p_stmt.add_param(tag);

		statements.push_back(p_stmt);
	}

	return statements;
}