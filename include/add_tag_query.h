#ifndef _ADD_TAG_QUERY_H_
#define _ADD_TAG_QUERY_H_

#include <string>
#include "prepared_statement.h"

class Add_tag_query {
public:
	void set_tag_name(const std::string& tag_name);
	void set_parent_id(const std::string& parent_id);

	const std::string& get_tag_name() const;

	Prepared_statement create_tag_statement() const;
	Prepared_statement create_tree_statement(const std::string& last_id) const;
private:
	std::string tag_name_;
	std::string parent_id_;
};

#endif