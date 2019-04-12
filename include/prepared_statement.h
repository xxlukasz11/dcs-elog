#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <string>
#include <vector>

class Prepared_statement{
public:
	using params_type = std::vector<std::string>;
	Prepared_statement() = default;
	Prepared_statement(std::string query_string);

	void set_query(std::string query);
	void add_param(std::string param);
	Prepared_statement& operator+=(std::string query_part);

	const std::string& get_sql() const;
	const params_type& get_params() const;
private:
	std::string query_string_;
	params_type params_;
};

#endif