#include <string>
#include <vector>
#include <utility>
#include "utils.h"
#include "prepared_statement.h"
#include "select_query.h"

namespace {
const std::string EVENT_IDS_PLACEHOLDER = "$event_ids";
const auto EVENT_IDS_PLACEHOLDER_LENGTH = EVENT_IDS_PLACEHOLDER.size();

namespace sql {
	const std::string before_tags_where = "SELECT Events.Id, datetime(Events.Date, 'unixepoch') AS Date, Events.Title AS Title, Events.Description AS Description, group_concat(Tags_list.Tag) AS Tags, Events.author AS Author FROM ( SELECT DISTINCT events.id FROM Events JOIN Tags_of_events ON Events.id = Tags_of_events.Event_id JOIN ( WITH RECURSIVE nodes(tag_id) AS ( SELECT Tags_tree.id FROM Tags_tree WHERE Tags_tree.id IN (SELECT id FROM Tags_list";
	const std::string after_tags_where = ") UNION ALL SELECT Tags_tree.id FROM Tags_tree JOIN nodes ON Tags_tree.parent_id = nodes.tag_id) SELECT Tags_list.id AS id FROM Tags_list JOIN (SELECT DISTINCT tag_id FROM nodes) tmp_table ON tmp_table.tag_id = Tags_list.id) tags_ids ON Tags_of_events.Tag_id = tags_ids.id) sub JOIN Tags_of_events ON sub.Id = Tags_of_events.Event_id JOIN Tags_list ON Tags_of_events.Tag_id = Tags_list.id JOIN Events ON sub.Id = Events.Id";
	const std::string after_date_where = " GROUP BY sub.Id ORDER BY Events.Date DESC";
	const std::string limit = " LIMIT ?";
	const std::string offset = " OFFSET ?";
	const std::string tag_sql = "Tags_list.tag = ?";
	const std::string min_date_sql = "Events.Date >= strftime('%s', ?)";
	const std::string max_date_sql = "Events.Date <= strftime('%s', ?)";

	const std::string select_attachments = "SELECT Id, Event_id, Name, Type, File_name FROM Attachments WHERE Event_id in (" + EVENT_IDS_PLACEHOLDER + ")";
}
}

void Select_query::set_min_date(const std::string& date){
	if (!date.empty()) {
		min_date_ = date;
		min_date_is_set_ = true;
	}
}

void Select_query::set_max_date(const std::string& date){
	if (!date.empty()) {
		max_date_ = date;
		max_date_is_set_ = true;
	}
}

void Select_query::create_tag(const std::string& tag){
	tags_.push_back(tag);
}

void Select_query::set_tags(std::vector<std::string>&& tags){
	tags_ = std::move(tags);
}

void Select_query::set_limit(const std::string& limit) {
	limit_ = limit;
}

void Select_query::set_offset(const std::string& offset) {
	offset_ = offset;
}

Prepared_statement Select_query::create_statement() const {
	std::vector<std::string> and_conds;
	std::vector<std::string> or_conds;
	Prepared_statement p_stmt;

	for (const auto& tag : tags_) {
		or_conds.push_back(sql::tag_sql);
		p_stmt.add_param(tag);
	}

	if (min_date_is_set_) {
		and_conds.push_back(sql::min_date_sql);
		p_stmt.add_param(min_date_);
	}

	if (max_date_is_set_) {
		and_conds.push_back(sql::max_date_sql);
		p_stmt.add_param(max_date_);
	}

	p_stmt.set_query(sql::before_tags_where);

	if(!or_conds.empty()) {
		p_stmt += " WHERE ";

		const size_t n_or_conds = or_conds.size();
		if (n_or_conds > 1)
			p_stmt += "(";
		for (size_t i = 0; i < n_or_conds; ++i) {
			p_stmt += or_conds[i];
			if (i != n_or_conds - 1)
				p_stmt += " OR ";
		}
		if (n_or_conds > 1)
			p_stmt += ")";
	}

	p_stmt += sql::after_tags_where;

	if(!and_conds.empty()){
		p_stmt += " WHERE ";
		if (and_conds.size() > 1)
			p_stmt += "(";
		const size_t n_conds = and_conds.size();
		for (size_t i = 0; i < n_conds; ++i) {
			p_stmt += and_conds[i];
			if (i != n_conds - 1)
				p_stmt += " AND ";
		}
		if (and_conds.size() > 1)
			p_stmt += ")";
	}
	
	p_stmt += sql::after_date_where;

	if (!limit_.empty()) {
		p_stmt += sql::limit;
		p_stmt.add_param(limit_);
	}
	
	if (!offset_.empty()) {
		p_stmt += sql::offset;
		p_stmt.add_param(offset_);
	}

	p_stmt.set_end();
	return p_stmt;
}

Prepared_statement Select_query::create_attachments_statement(const std::vector<std::string>& event_ids) const {
	std::string query_string = create_attachment_query_string(event_ids);
	auto stmt = Prepared_statement(query_string);
	stmt.add_params(event_ids);
	return stmt;
}

std::string Select_query::create_attachment_query_string(const std::vector<std::string>& event_ids) const {
	std::string id_list = utils::create_list_string(Prepared_statement::PARAM_PLACEHOLDER, event_ids.size());
	std::string query_string = sql::select_attachments;
	auto placeholder_pos = query_string.find(EVENT_IDS_PLACEHOLDER);
	query_string.replace(placeholder_pos, EVENT_IDS_PLACEHOLDER_LENGTH, id_list);
	return query_string;
}
