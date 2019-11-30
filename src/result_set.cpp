#include <utility>
#include <algorithm>
#include <string>
#include <vector>

#include "result_set.h"

bool Result_set::is_header_empty() const {
	return !header_set_;
}

void Result_set::set_header(char** c_header, int c_header_size){
	for(int i = 0; i < c_header_size; ++i)
		this->header_.emplace_back(c_header[i]);
	header_set_ = true;
}

void Result_set::insert_row(char** c_data, int c_data_size){
	std::vector<std::string> row;
	for(int i = 0; i < c_data_size; ++i){
		if ( c_data[i] == nullptr )
			row.emplace_back();
		else
			row.emplace_back(c_data[i]);
	}
	data_.push_back( std::move(row) );
}

void Result_set::insert_row(std::vector<std::string>&& row){
	data_.push_back(std::move(row));
}

void Result_set::set_header(std::vector<std::string>&& row){
	header_ = std::move(row);
	header_set_ = true;
}


std::string Result_set::get_first_field() const {
	if (!data_.empty() && !data_[0].empty()) {
		return data_[0][0];
	}
	return std::string{};
}

std::string Result_set::get_field(int row_index, int column_index) {
	const auto& row = get_row(row_index);
	if (column_index < row.size()) {
		return row[column_index];
	}
	return {};
}

const Result_set::header_type& Result_set::get_header() const {
	return header_;
}

Result_set::header_type& Result_set::get_header() {
	return header_;
}


const Result_set::data_type& Result_set::get_data() const {
	return data_;
}

Result_set::column_type Result_set::get_column(size_t index) const {
	Result_set::column_type column;
	if (data_.size() == 0 || data_[0].size() <= index || index < 0) {
		return column;
	}
	column.reserve(data_.size());
	for (const auto& row : data_) {
		column.push_back( row[index] );
	}
	return column;
}

const Result_set::row_type& Result_set::get_row(size_t index) const {
	return data_[index];
}

void Result_set::set_last_row_id(std::string id){
	last_row_id_ = id;
}

size_t Result_set::data_size() const {
	return data_.size();
}

size_t Result_set::header_length() const {
	return header_.size();
}

std::string Result_set::get_last_row_id() const {
	return last_row_id_;
}

bool Result_set::has_records() const {
	return data_.size() > 0;
}

Result_set::row_type& Result_set::operator[](size_t index) {
	return data_[index];
}

Result_set& Result_set::operator+=(Result_set&& result_set){
	data_.reserve(data_.size() + result_set.data_.size());
	std::move(result_set.data_.begin(), result_set.data_.end(), std::back_inserter(data_));
	return *this;
}

Result_set Result_set::copy_all_except_data() const {
	Result_set output;
	output.header_ = header_;
	output.last_row_id_ = last_row_id_;
	output.header_set_ = header_set_;
	return output;
}

std::ostream& operator<<(std::ostream& stream, const Result_set& result_set){
	// determine max width of each column
	const auto N = result_set.header_.size();
	std::vector<size_t> lengths(N);

	for(size_t i = 0; i < N; ++i)
		lengths[i] = result_set.header_[i].length();

	for(const auto& row : result_set.data_){
		for(size_t i = 0; i < N; ++i)
			lengths[i] = std::max( row[i].length(), lengths[i] );
	}

	// print Result_set
	for(size_t i = 0; i < N; ++i){
		stream << " | ";
		std::cout.width(lengths[i]);
		stream << std::internal << result_set.header_[i];
	}
	stream << " | \n";

	for(const auto& row : result_set.data_){
		for(size_t i = 0; i < N; ++i){
			stream << " | ";
			std::cout.width(lengths[i]);
			stream << std::internal << row[i];
		}
		stream << " | \n";
	}

	return stream;
}
