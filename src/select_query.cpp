#include <string>
#include <vector>
#include <utility>

#include "prepared_statement.h"
#include "select_query.h"

namespace sql_string{
	static const std::string before_where = "SELECT Events.Id, datetime(Events.Date, 'unixepoch') as Date, Events.Title, Events.Description, group_concat(Tags.Tag) as Tags FROM (SELECT DISTINCT events.id FROM Events JOIN Tags ON Events.id = Tags.Id";
	static const std::string after_where = ") sub JOIN Tags ON sub.Id = Tags.Id JOIN Events ON sub.Id = Events.Id GROUP BY sub.Id;";
}


void Select_query::set_min_date(std::string date){
	min_date_ = date;
	min_date_is_set_ = true;
}

void Select_query::set_max_date(std::string date){
	max_date_ = date;
	max_date_is_set_ = true;
}

void Select_query::add_tag(std::string tag){
	tags_.push_back(tag);
}

void Select_query::set_tags(std::vector<std::string>&& tags){
	tags_ = std::move(tags);
}

std::string Select_query::create_sql() const {
	std::vector<std::string> conds;

	if(min_date_is_set_){
		std::string sql =  "Events.Date >= strftime('%s', '";
		sql += min_date_;
		sql += "')";
		conds.push_back(sql);
	}

	if(max_date_is_set_){
		std::string sql =  "Events.Date <= strftime('%s', '";
		sql += max_date_;
		sql += "')";
		conds.push_back(sql);
	}

	if(!tags_.empty()) {
		std::vector<std::string> or_conds;
		for(const auto& tag : tags_){
			std::string sql = "Tags.Tag = '";
			sql += tag + "'";
			or_conds.push_back(sql);
		}

		std::string or_query;
		size_t n_or_conds = or_conds.size();

		if(n_or_conds > 1)
			or_query += "(";

		for(size_t i = 0; i < n_or_conds; ++i){
			or_query += or_conds[i];
			if(i != n_or_conds -1)
				or_query += " OR ";
		}

		if(n_or_conds > 1)
			or_query += ")";
		conds.push_back(or_query);
	}

	std::string query = sql_string::before_where;

	if(conds.empty())
		return (query += sql_string::after_where);
	
	query += " WHERE ";

	size_t n_conds = conds.size();
	for(size_t i = 0; i < n_conds; ++i){
		query += conds[i];
		if(i != n_conds-1)
			query += " AND ";
	}
	
	return (query += sql_string::after_where);
}

Prepared_statement Select_query::create_statement() const {
	std::vector<std::string> conds;
	Prepared_statement p_stmt;

	if(min_date_is_set_){
		std::string sql =  "Events.Date >= strftime('%s', ?)";
		conds.push_back(sql);
		p_stmt.add_param(min_date_);
	}

	if(max_date_is_set_){
		std::string sql =  "Events.Date <= strftime('%s', ?)";
		conds.push_back(sql);
		p_stmt.add_param(max_date_);
	}

	if(!tags_.empty()) {
		std::vector<std::string> or_conds;
		for(const auto& tag : tags_){
			std::string sql = "Tags.Tag = ?";
			or_conds.push_back(sql);
			p_stmt.add_param(tag);
		}

		std::string or_query;
		size_t n_or_conds = or_conds.size();

		if(n_or_conds > 1)
			or_query += "(";

		for(size_t i = 0; i < n_or_conds; ++i){
			or_query += or_conds[i];
			if(i != n_or_conds -1)
				or_query += " OR ";
		}

		if(n_or_conds > 1)
			or_query += ")";

		conds.push_back(or_query);
	}

	p_stmt.set_query(sql_string::before_where);


	if(conds.empty()){
		p_stmt += sql_string::after_where;
		return p_stmt;
	}
	
	p_stmt += " WHERE ";

	size_t n_conds = conds.size();
	for(size_t i = 0; i < n_conds; ++i){
		p_stmt += conds[i];
		if(i != n_conds-1)
			p_stmt += " AND ";
	}
	
	p_stmt += sql_string::after_where;

	return p_stmt;
}