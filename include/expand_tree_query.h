#ifndef _TAGS_TREE_EXPANDER_H_
#define _TAGS_TREE_EXPANDER_H_

#include <vector>
#include <string>
#include "prepared_statement.h"

class Expand_tree_query {
public:
	Expand_tree_query(const std::vector<std::string>& tags_vector);
	Prepared_statement create_statement();

private:
	std::vector<std::string> tags_;
};

#endif