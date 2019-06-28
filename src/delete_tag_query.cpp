#include "delete_tag_query.h"

namespace {
	namespace sql {
		std::string update_tree = "update tags_tree set parent_id = (select parent_id from tags_tree where id = ?) where parent_id = ?;";
		std::string delete_from_tree = "DELETE from Tags_tree WHERE id = ?";
		std::string delete_from_list = "DELETE FROM Tags_list WHERE id = ?";
		std::string select_tag_name = "SELECT tag FROM Tags_list WHERE id = ?";
	}
}

void Delete_tag_query::set_tag_id(const std::string& tag_id) {
	tag_id_ = tag_id;
}

const std::string& Delete_tag_query::get_tag_id() const {
	return tag_id_;
}

Prepared_statement Delete_tag_query::create_select_statement() const {
	Prepared_statement stmt{ sql::select_tag_name };
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::create_delete_tree_statement() const {
	Prepared_statement stmt{ sql::delete_from_tree };
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::create_update_tree_statement() const {
	Prepared_statement stmt{ sql::update_tree };
	stmt.add_param(tag_id_);
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Delete_tag_query::create_list_statement() const {
	Prepared_statement stmt{ sql::delete_from_list };
	stmt.add_param(tag_id_);
	return stmt;
}
