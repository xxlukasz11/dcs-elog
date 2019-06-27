#include "delete_tag_query.h"

namespace sql_string {
	std::string delete_from_tree = "update tags_tree set parent_id = (select parent_id from tags_tree where id = ?) where parent_id = ?;";
	std::string delete_from_list = "DELETE FROM Tags_list WHERE id = ?";
	std::string select_tag_name = "SELECT tag FROM Tags_list WHERE id = ?";
}

void Delete_tag_query::set_tag_id(int tag_id) {
	tag_id_ = std::to_string(tag_id);
}

Prepared_statement Delete_tag_query::create_select_statement() const {
	Prepared_statement stmt{ sql_string::select_tag_name };
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::create_tree_statement() const {
	Prepared_statement stmt{ sql_string::delete_from_tree };
	stmt.add_param(tag_id_);
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::create_list_statement() const {
	Prepared_statement stmt{ sql_string::delete_from_list };
	stmt.add_param(tag_id_);
	return stmt;
}
