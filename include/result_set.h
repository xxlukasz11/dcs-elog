#ifndef _RESULT_SET_H_
#define _RESULT_SET_H_

#include <vector>
#include <string>
#include <iostream>

class Result_set{
public:
	using column_type = std::vector<std::string>;
	using row_type = std::vector<std::string>;
	using data_type = std::vector<row_type>;
	using header_type = row_type;
	
	bool is_header_empty() const;
	void set_header(char** c_header, int c_header_size);
	void set_header(std::vector<std::string>&& row);
	void insert_row(char** c_data, int c_data_size);
	void insert_row(std::vector<std::string>&& row);
	void set_last_row_id(std::string id);

	size_t data_size() const;
	size_t header_length() const;
	std::string get_last_row_id() const;
	bool has_records() const;
	std::string get_first_field() const;
	const header_type& get_header() const;
	header_type& get_header();
	const data_type& get_data() const;
	column_type get_column(size_t index);
	const row_type& get_row(size_t index) const;

	row_type& operator[](size_t index);
	Result_set& operator+=(Result_set&& result_set);
	friend std::ostream& operator<<(std::ostream& stream, const Result_set& result_set);

private:
	std::string last_row_id;

	bool header_set_{ false };
	header_type header_;
	data_type data_;
};

std::ostream& operator<<(std::ostream& stream, const Result_set& result_set);

#endif
