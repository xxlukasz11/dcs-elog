#include "select_tags_query.h"

std::string Select_tags_query::create_sql() const {
	std::string sql = "SELECT tmp.tag AS tag, Tags_list.tag AS parent FROM (SELECT Tags_list.id, Tags_list.tag, Tags_tree.parent_id FROM Tags_list JOIN Tags_tree ON Tags_tree.id = Tags_list.id) tmp LEFT JOIN Tags_list ON Tags_list.id = tmp.parent_id";
	return sql;
}