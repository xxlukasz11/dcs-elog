#include "prepared_statement.h"
#include "expand_tree_query.h"

namespace {
	namespace sql {
		std::string before_tags = "WITH RECURSIVE nodes(tag_id) AS (SELECT Tags_tree.id FROM Tags_tree WHERE Tags_tree.id IN (SELECT id FROM Tags_list WHERE ";
		std::string after_tags = ") UNION ALL SELECT Tags_tree.id FROM Tags_tree JOIN nodes ON Tags_tree.parent_id = nodes.tag_id) SELECT tag FROM Tags_list JOIN (SELECT DISTINCT tag_id FROM nodes) tmp_table ON tmp_table.tag_id = Tags_list.id;";
		std::string tag_str = "Tags_list.tag = ? ";
	}
}

Expand_tree_query::Expand_tree_query(const std::vector<std::string>& tags_vector) : tags_(tags_vector) {
}

Prepared_statement Expand_tree_query::create_statement() {
	Prepared_statement stmt(sql::before_tags);

	int n = tags_.size();
	for (int i = 0; i < n; ++i) {
		stmt += sql::tag_str;
		if (i != n-1)
			stmt += "OR ";
		stmt.add_param(tags_[i]);
	}

	stmt += sql::after_tags;
	return stmt;
}
