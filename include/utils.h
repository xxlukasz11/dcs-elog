#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <ctime>
#include "log_level.h"
#include "attachment_info.h"

namespace utils{
	using String_array = std::vector<std::string>;
	using Pair_array = std::vector<std::pair<std::string, std::string>>;
	using Attachment_info_array = std::vector<Attachment_info>;

	// Concatenate array of strings with ", " delimiter
	std::string concatenate_string_array(const String_array& array);

	// Excludes elements from base which are present in pattern
	String_array exclude_from_array(const String_array& base, const String_array& pattern);

	// Converts time parameter to string according to format parameter.
	std::string create_date_time_string(time_t time, const std::string& format);

	// converts comma and space separated string to std::vector
	String_array string_to_vector(std::string string);

	// converts delimiter separated string to std::vector
	String_array string_to_vector(const std::string& string, const std::string& delimiter);

	/*
	converts space separated string of pairs to std::vector<pair>
	pair format: "<value_1_1><separator><value_1_2> <value_2_1><separator><value_2_2>"
	*/
	Pair_array string_to_pair_array(char pair_separator, std::string string);

	// creates attachment objects from pair values
	Attachment_info_array pair_array_to_attachment(const Pair_array& pair_array);

	// Converts string to Log_level enum. Returns -1 on error.
	Log_level decode_log_level(const std::string& string);

	// Converts std::string to T
	template<typename T>
	T convert_string(const std::string& string);

	// creates string of "," delimited list of expressions
	std::string create_list_string(const std::string& expression, int list_size);
}

#endif