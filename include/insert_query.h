#ifndef _INSERT_QUERY_H_
#define _INSERT_QUERY_H_

#include <vector>
#include <string>
#include "prepared_statement.h"
#include "attachment_database_info.h"
#include "database.h"

class Insert_query{
public:

	void set_title(const std::string& title);
	void set_desc(const std::string& desc);
	void create_tag(const std::string& tag);
	void set_tags(std::vector<std::string>&& tags);
	void set_author(const std::string& author);
	void set_attachments(const Attachment_database_info_array& attachment_info);

	const std::vector<std::string>& get_tags() const;
	bool has_attachments() const;

	Prepared_statement create_tags_exist_statement() const;
	Prepared_statement create_events_statement() const;
	Prepared_statement create_tags_statement(const std::string& event_id) const;
	Prepared_statement create_attachments_statement(const std::string& event_id) const;
private:

	bool title_is_set_{ false };
	bool desc_is_set_{ false };
	bool author_is_set_{ false };

	std::string title_;
	std::string desc_;
	std::vector<std::string> tags_;
	std::string author_;
	Attachment_database_info_array attachment_array_;
};

#endif