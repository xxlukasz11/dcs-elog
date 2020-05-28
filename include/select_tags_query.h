#ifndef _SELECT_TAGS_QUERY_H_
#define _SELECT_TAGS_QUERY_H_

#include <string>

/*
Creates prepared statements for reading tags tree
*/
class Select_tags_query {
public:
	std::string create_sql() const;
};

#endif