#include "prepared_statement.h"
#include "expand_tree_query.h"

namespace sql_string {
	std::string before_tags = "WITH RECURSIVE nodes(tag) AS ( SELECT Tags_tree.tag FROM Tags_tree WHERE ";
	std::string after_tags = "UNION ALL SELECT Tags_tree.tag FROM Tags_tree JOIN nodes ON Tags_tree.parent = nodes.tag ) SELECT DISTINCT tag FROM nodes;";
	std::string tag_str = "Tags_tree.tag = ? ";
}

Expand_tree_query::Expand_tree_query(const std::vector<std::string>& tags_vector) : tags_(tags_vector) {
}

Prepared_statement Expand_tree_query::create_statement() {
	Prepared_statement stmt(sql_string::before_tags);

	int n = tags_.size();
	for (int i = 0; i < n; ++i) {
		stmt += sql_string::tag_str;
		if (i != n-1)
			stmt += "OR ";
		stmt.add_param(tags_[i]);
	}

	stmt += sql_string::after_tags;
	return stmt;
}
