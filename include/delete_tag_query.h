#ifndef _DELETE_TAG_QUERY_H_
#define _DELETE_TAG_QUERY_H_

#include <string>
#include "prepared_statement.h"

/*
Creates prepared statements for tag deletion
*/
class Delete_tag_query {
public:
	Delete_tag_query();
	void set_tag_id(const std::string& tag_id);
	const std::string& get_tag_id() const;

	enum value {
		ALLOW_NULL,
		RETURN_DEFAULT
	};

	Prepared_statement select_statement() const;
	Prepared_statement parent_id_statement(value mode) const;

	Prepared_statement delete_events_tag_statement(const std::string& parent_id) const;
	Prepared_statement update_events_tag_statement(const std::string& parent_id) const;
	Prepared_statement delete_redundant_statement() const;

	Prepared_statement update_tree_statement(const std::string& parent_id) const;
	Prepared_statement delete_tree_statement() const;
	Prepared_statement delete_list_statement() const;
private:
	std::string tag_id_;
};

#endif