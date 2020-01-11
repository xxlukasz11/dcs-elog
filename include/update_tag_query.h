#ifndef _UPDATE_TAG_QUERY_H_
#define _UPDATE_TAG_QUERY_H_

#include <string>
#include "prepared_statement.h"

class Update_tag_query {
public:
	void set_tag_id(const std::string& tag_id);
	void set_tag_name(const std::string& tags_name);

	const std::string& get_tag_name() const;
	const std::string& get_new_tag_name() const;

	Prepared_statement create_select_tag_statement() const;
	Prepared_statement create_select_new_tag_statement() const;
	Prepared_statement create_update_statement() const;
private:
	bool tag_id_is_set_{ false };
	std::string tag_id_;
	bool tag_name_is_set_{ false };
	std::string tag_name_;
};

#endif
