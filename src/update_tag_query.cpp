#include "update_tag_query.h"
#include "custom_exceptions.h"

namespace {
	namespace sql {
		std::string select_tag_name = "SELECT tag FROM Tags_list WHERE id = ?;";
		std::string update_tag_name = "UPDATE Tags_list SET tag = ? WHERE id = ?;";
	}
}

void Update_tag_query::set_tag_id(const std::string & tag_id) {
	tag_id_ = tag_id;
	tag_id_is_set_ = true;
}

void Update_tag_query::set_tag_name(const std::string & tags_name) {
	tag_name_ = tags_name;
	tag_name_is_set_ = true;
}

const std::string & Update_tag_query::get_tag_name() const {
	return tag_name_;
}

Prepared_statement Update_tag_query::create_select_tag_statement() const {
	if (!tag_id_is_set_) {
		throw Query_error("Update tag error. Tag id is not set.");
	}

	Prepared_statement stmt{ sql::select_tag_name };
	stmt.add_param(tag_id_);
	return stmt;
}

Prepared_statement Update_tag_query::create_update_statement() const {
	if (!tag_id_is_set_ || !tag_name_is_set_) {
		throw Query_error("Update tag error. One of the fields is not set");
	}

	Prepared_statement stmt{ sql::update_tag_name };
	stmt.add_param(tag_name_);
	stmt.add_param(tag_id_);

	return stmt;
}
