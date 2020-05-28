#ifndef _TAGS_TREE_EXPANDER_H_
#define _TAGS_TREE_EXPANDER_H_

#include <vector>
#include <string>
#include "prepared_statement.h"

/*
Creates prepared statements for tags tree unwinding
Not used anymore, since handling is moved to return_events_query
*/
class Expand_tree_query {
public:
	Expand_tree_query(const std::vector<std::string>& tags_vector);
	Prepared_statement create_statement();

private:
	std::vector<std::string> tags_;
};

#endif