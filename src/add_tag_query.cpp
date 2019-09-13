#include <stdexcept>

#include "add_tag_query.h"

void Add_tag_query::set_tag_name(const std::string& tag_name) {
	tag_name_ = tag_name;
}

void Add_tag_query::set_parent_id(const std::string& parent_id) {
	parent_id_ = parent_id;
}

const std::string& Add_tag_query::get_tag_name() const {
	return tag_name_;
}

Prepared_statement Add_tag_query::create_get_tag_id_statement() const {
	Prepared_statement stmt{ "SELECT id FROM Tags_list WHERE tag = ?;" };
	stmt.add_param(tag_name_);
	return stmt;
}

Prepared_statement Add_tag_query::create_tag_statement() const {
	Prepared_statement stmt{ "INSERT INTO Tags_list VALUES(null, ?);" };
	stmt.add_param(tag_name_);
	return stmt;
}

Prepared_statement Add_tag_query::create_tree_statement(const std::string& last_id) const {
	Prepared_statement stmt{ "INSERT INTO Tags_tree VALUES(?, ?);" };
	stmt.add_param(last_id);
	stmt.add_param(parent_id_);
	return stmt;
}