#include <string>
#include <vector>
#include <utility>

#include "prepared_statement.h"
#include "select_query.h"

namespace {
	namespace sql {
		static const std::string before_tags_where = "SELECT Events.Id, datetime(Events.Date, 'unixepoch') AS Date, Events.Title AS Title, Events.Description AS Description, group_concat(Tags_list.Tag) AS Tags, Events.author AS Author FROM ( SELECT DISTINCT events.id FROM Events JOIN Tags_of_events ON Events.id = Tags_of_events.Event_id JOIN ( WITH RECURSIVE nodes(tag_id) AS ( SELECT Tags_tree.id FROM Tags_tree WHERE Tags_tree.id IN (SELECT id FROM Tags_list";
		static const std::string after_tags_where = ") UNION ALL SELECT Tags_tree.id FROM Tags_tree JOIN nodes ON Tags_tree.parent_id = nodes.tag_id) SELECT Tags_list.id AS id FROM Tags_list JOIN (SELECT DISTINCT tag_id FROM nodes) tmp_table ON tmp_table.tag_id = Tags_list.id) tags_ids ON Tags_of_events.Tag_id = tags_ids.id) sub JOIN Tags_of_events ON sub.Id = Tags_of_events.Event_id JOIN Tags_list ON Tags_of_events.Tag_id = Tags_list.id JOIN Events ON sub.Id = Events.Id";
		static const std::string after_date_where = " GROUP BY sub.Id ORDER BY Events.Date DESC;";
		static const std::string tag_sql = "Tags_list.tag = ?";
		static const std::string min_date_sql = "Events.Date >= strftime('%s', ?)";
		static const std::string max_date_sql = "Events.Date <= strftime('%s', ?)";
	}
}

void Select_query::set_min_date(std::string date){
	min_date_ = date;
	min_date_is_set_ = true;
}

void Select_query::set_max_date(std::string date){
	max_date_ = date;
	max_date_is_set_ = true;
}

void Select_query::create_tag(std::string tag){
	tags_.push_back(tag);
}

void Select_query::set_tags(std::vector<std::string>&& tags){
	tags_ = std::move(tags);
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

		size_t n_or_conds = or_conds.size();
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
		size_t n_conds = and_conds.size();
		for (size_t i = 0; i < n_conds; ++i) {
			p_stmt += and_conds[i];
			if (i != n_conds - 1)
				p_stmt += " AND ";
		}
		if (and_conds.size() > 1)
			p_stmt += ")";
	}
	
	p_stmt += sql::after_date_where;

	return p_stmt;
}