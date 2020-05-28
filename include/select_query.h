#ifndef _SELECT_QUERY_H_
#define _SELECT_QUERY_H_

#include <vector>
#include <string>
#include "prepared_statement.h"

/*
Creates prepared statements for events reading
*/
class Select_query{
public:
	void set_min_date(const std::string& date);
	void set_max_date(const std::string& date);
	void create_tag(const std::string& tag);
	void set_tags(std::vector<std::string>&& tags);
	void set_limit(const std::string& limit);
	void set_offset(const std::string& offset);

	Prepared_statement create_statement() const;
	Prepared_statement create_attachments_statement(const std::vector<std::string>& event_ids) const;
private:
	std::string create_attachment_query_string(const std::vector<std::string>& event_ids) const;
	bool min_date_is_set_{ false };
	bool max_date_is_set_{ false };

	std::string min_date_;
	std::string max_date_;
	std::vector<std::string> tags_;
	std::string limit_;
	std::string offset_;
};

#endif