#ifndef _INSERT_QUERY_H_
#define _INSERT_QUERY_H_

#include <vector>
#include <string>

#include "prepared_statement.h"
#include "database.h"

class Insert_query{
public:

	void set_title(std::string title);
	void set_desc(std::string desc);
	void add_tag(std::string tag);
	void set_tags(std::vector<std::string>&& tags);

	std::string create_events_sql() const;
	std::string create_tags_sql(const std::string& event_id) const;

	Prepared_statement create_events_statement() const;
	std::vector<Prepared_statement> create_tags_statements(const std::string& event_id) const;
private:

	bool title_is_set_{ false };
	bool desc_is_set_{ false };

	std::string title_;
	std::string desc_;
	std::vector<std::string> tags_;
};

#endif