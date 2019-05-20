#include "select_tags_query.h"

std::string Select_tags_query::create_sql() const {
	std::string sql = "SELECT * FROM Tags_tree";
	return sql;
}