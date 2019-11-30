#ifndef _SELECT_QUERY_H_
#define _SELECT_QUERY_H_

#include <vector>
#include <string>
#include "prepared_statement.h"

class Select_query{
public:
	void set_min_date(std::string date);
	void set_max_date(std::string date);
	void create_tag(std::string tag);
	void set_tags(std::vector<std::string>&& tags);

	Prepared_statement create_statement() const;
	Prepared_statement create_attachments_statement(const std::vector<std::string>& event_ids) const;
private:
	std::string create_attachment_query_string(const std::vector<std::string>& event_ids) const;
	bool min_date_is_set_{ false };
	bool max_date_is_set_{ false };

	std::string min_date_;
	std::string max_date_;
	std::vector<std::string> tags_;
};

#endif