#include "update_tag_query.h"
#include "custom_exceptions.h"

void Update_tag_query::set_tag_id(const std::string & tag_id) {
	tag_id_ = tag_id;
	tag_id_is_set_ = true;
}

void Update_tag_query::set_tag_name(const std::string & tags_name) {
	tag_name_ = tags_name;
	tag_name_is_set_ = true;
}

Prepared_statement Update_tag_query::create_update_statement() const {
	if (!tag_id_is_set_ || !tag_name_is_set_) {
		throw Query_error("Update tag error. One of the fields is not set");
	}

	Prepared_statement stmt{ "UPDATE Tags_list SET tag = ? WHERE id = ?" };
	stmt.add_param(tag_name_);
	stmt.add_param(tag_id_);

	return stmt;
}
