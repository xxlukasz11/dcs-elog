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


const Result_set::header_type& Result_set::get_header() const {
	return header_;
}


const Result_set::data_type& Result_set::get_data() const {
	return data_;
}

void Result_set::set_last_row_id(std::string id){
	last_row_id = id;
}

std::string Result_set::get_last_row_id() const {
	return last_row_id;
}

Result_set& Result_set::operator+=(Result_set&& result_set){
	data_.reserve(data_.size() + result_set.data_.size());
	std::move(result_set.data_.begin(), result_set.data_.end(), std::back_inserter(data_));

	return *this;
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
