#include <utility>
#include <string>
#include <vector>

#include "custom_exceptions.h"
#include "insert_query.h"
#include "administrator.h"

namespace {
	namespace sql {
		std::string tags_exist_before = "SELECT input.tag FROM (SELECT ? AS tag";
		std::string tags_exist_after = ") input WHERE input.tag NOT IN (SELECT tag FROM Tags_list);";
		std::string tags_exist_union = " UNION SELECT ?";

		std::string insert_event = "INSERT INTO Events VALUES(null, strftime('%s'), ?, ?, ?);";
		std::string insert_tags_before = "INSERT INTO Tags_of_events(Event_id, Tag_id) SELECT ?, Tags_list.id FROM Tags_list WHERE ";
		std::string insert_tags_condition = "Tags_list.tag = ?";

		std::string insert_attachments_before = "INSERT INTO Attachments(Event_id, Name, Type, File_name) SELECT ?, ?, ?, ?";
		std::string insert_attachments_union = " UNION ALL SELECT ?, ?, ?, ?";
	}
}

void Insert_query::set_title(const std::string& title) {
	title_ = title;
	title_is_set_ = true;
}

void Insert_query::set_desc(const std::string& desc) {
	desc_ = desc;
	desc_is_set_ = true;
}

void Insert_query::create_tag(const std::string& tag) {
	tags_.push_back(tag);
}

void Insert_query::set_tags(std::vector<std::string>&& tags){
	tags_ = std::move(tags);
	if (tags_.empty()) {
		auto empty_tag_name = Administrator::instance().params().get_empty_tag_name();
		tags_.push_back(empty_tag_name);
	}
}

void Insert_query::set_author(const std::string& author) {
	author_ = author;
	author_is_set_ = true;
}

void Insert_query::set_attachments(const Attachment_database_info_array& attachment_array) {
	attachment_array_ = attachment_array;
}

const std::vector<std::string>& Insert_query::get_tags() const {
	return tags_;
}

bool Insert_query::has_attachments() const {
	return attachment_array_.size() > 0;
}

Prepared_statement Insert_query::create_tags_exist_statement() const {
	size_t n_of_tags = tags_.size();
	if (n_of_tags == 0)
		return Prepared_statement();

	Prepared_statement stmt{ sql::tags_exist_before };
	stmt.add_param(tags_[0]);

	for (size_t i = 1; i < n_of_tags; ++i) {
		stmt += sql::tags_exist_union;
		stmt.add_param(tags_[i]);
	}

	stmt += sql::tags_exist_after;
	return stmt;
}

Prepared_statement Insert_query::create_events_statement() const {
	if(!title_is_set_ && !desc_is_set_){
		throw Query_error("Insert query for Events table is empty");
	}

	Prepared_statement p_stmt{ sql::insert_event };
	p_stmt.add_param(title_);
	p_stmt.add_param(desc_);
	p_stmt.add_param(author_);
	return p_stmt;
}

Prepared_statement Insert_query::create_tags_statement(const std::string& event_id) const {
	Prepared_statement stmt{ sql::insert_tags_before };
	stmt.add_param(event_id);

	size_t n_tags = tags_.size();
	for (size_t i = 0; i < n_tags; ++i) {
		if (i != 0)
			stmt += " OR ";
		stmt += sql::insert_tags_condition;
		stmt.add_param(tags_[i]);
	}
	stmt += ";";
	return stmt;
}

Prepared_statement Insert_query::create_attachments_statement(const std::string& event_id) const {
	if (attachment_array_.size() == 0) {
		return Prepared_statement{};
	}

	Prepared_statement stmt{ sql::insert_attachments_before };
	stmt.add_param(event_id);
	stmt.add_param(attachment_array_[0].get_name());
	stmt.add_param(attachment_array_[0].get_type());
	stmt.add_param(attachment_array_[0].get_file_name());

	for (size_t i = 1; i < attachment_array_.size(); ++i) {
		
		stmt += sql::insert_attachments_union;
		stmt.add_param(event_id);
		stmt.add_param(attachment_array_[i].get_name());
		stmt.add_param(attachment_array_[i].get_type());
		stmt.add_param(attachment_array_[i].get_file_name());
	}
	stmt += ";";
	return stmt;
}
