#include <utility>
#include <string>
#include <vector>

#include "custom_exceptions.h"
#include "insert_query.h"

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

std::string Insert_query::create_events_sql() const {
	if(!title_is_set_ && !desc_is_set_){
		throw Query_error("Insert query for Events table is empty");
	}

	std::string query{ "INSERT INTO Events VALUES(null, strftime('%s'), " };

	if(title_is_set_){
		query += "'";
		query += title_;
		query += "', ";
	}
	else
		query += "null, ";

	if(desc_is_set_){
		query += "'";
		query += desc_;
		query += "'";
	}
	else
		query += "null";

	query += ");";
	return query;
}

std::string Insert_query::create_tags_sql(const std::string& event_id) const {

	std::string multi_query;
	for(const auto& tag : tags_){
		std::string query{ "INSERT INTO Tags VALUES(" };
		query += event_id;
		query += ", '";
		query += tag;
		query += "'); ";
		multi_query += query;
	}

	return multi_query;
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
		p_stmt += "?";
		p_stmt.add_param(desc_);
	}
	else
		p_stmt += "null";

	p_stmt += ");";
	return p_stmt;
}


std::vector<Prepared_statement> Insert_query::create_tags_statements(const std::string& event_id) const {
	std::vector<Prepared_statement> statements;

	for(const auto& tag : tags_){
		Prepared_statement p_stmt{ "INSERT INTO Tags VALUES(?, ?);" };
		p_stmt.add_param(event_id);
		p_stmt.add_param(tag);

		statements.push_back(p_stmt);
	}

	return statements;
}