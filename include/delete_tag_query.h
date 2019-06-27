#ifndef _DELETE_TAG_QUERY_H_
#define _DELETE_TAG_QUERY_H_

#include <string>
#include "prepared_statement.h"

class Delete_tag_query {
public:
	void set_tag_id(const std::string& tag_id);
	const std::string& get_tag_id() const;

	Prepared_statement create_tree_statement() const;
	Prepared_statement create_list_statement() const;
	Prepared_statement create_select_statement() const;
private:
	std::string tag_id_;
};

#endif