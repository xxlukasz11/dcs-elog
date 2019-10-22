#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include "log_level.h"

namespace utils{
	using String_array = std::vector<std::string>;
	std::string concatenate_string_array(const String_array& array);
	String_array exclude_from_array(const String_array& base, const String_array& pattern);
	String_array string_to_vector(std::string string);
	Log_level decode_log_level(const std::string& string);

	template<typename T>
	T convert_string(const std::string& string);
}

#endif