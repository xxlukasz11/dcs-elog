#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include "log_level.h"
#include "attachment_info.h"

namespace utils{
	using String_array = std::vector<std::string>;
	using Pair_array = std::vector<std::pair<std::string, std::string>>;
	using Attachment_info_array = std::vector<Attachment_info>;

	std::string concatenate_string_array(const String_array& array);

	String_array exclude_from_array(const String_array& base, const String_array& pattern);

	// converts comma and space separated string to std::vector
	String_array string_to_vector(std::string string);

	// converts space separated string of pairs to std::vector<pair>
	Pair_array string_to_pair_array(char pair_separator, std::string string);

	// creates attachment objects from pair values
	Attachment_info_array pair_array_to_attachment(const Pair_array& pair_array);

	Log_level decode_log_level(const std::string& string);

	template<typename T>
	T convert_string(const std::string& string);
}

#endif