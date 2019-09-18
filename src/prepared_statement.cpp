#include <string>
#include <vector>

#include "prepared_statement.h"

Prepared_statement::Prepared_statement(std::string query_string)
	: query_string_(query_string) {}

void Prepared_statement::set_query(std::string query){
	query_string_ = query;
}

void Prepared_statement::add_param(std::string param){
	params_.push_back(param);
}

Prepared_statement& Prepared_statement::operator+=(std::string query_part){
	query_string_ += query_part;
	return *this;
}

const std::string& Prepared_statement::get_sql() const {
	return query_string_;
}

const Prepared_statement::params_type& Prepared_statement::get_params() const {
	return params_;
}