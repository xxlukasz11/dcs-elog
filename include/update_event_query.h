#ifndef _UPDATE_EVENT_QUERY_H_
#define _UPDATE_EVENT_QUERY_H_

#include <vector>
#include <string>

#include "prepared_statement.h"
#include "database.h"

class Update_event_query {
public:

	void set_id(const std::string& id);
	void set_title(const std::string& title);
	void set_description(const std::string& description);
	void set_tags(std::vector<std::string>&& tags);

	bool has_any_tags() const;

	Prepared_statement create_event_exists_stmt() const;
	Prepared_statement create_update_event_stmt() const;
	Prepared_statement create_delete_event_tags_statement() const;
private:

	bool id_is_set_{ false };
	bool title_is_set_{ false };
	bool description__is_set_{ false };

	std::string id_;
	std::string title_;
	std::string description_;
	std::vector<std::string> tags_;
};

#endif