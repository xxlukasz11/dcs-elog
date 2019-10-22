#include "utils.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>

namespace utils {

std::string concatenate_string_array(const String_array& array) {
	std::string text;
	size_t array_size = array.size();
	for (size_t i = 0; i < array_size; ++i) {
		text += array[i];
		if (i != array_size - 1)
			text += ", ";
	}
	return text;
}

String_array exclude_from_array(const String_array& base, const String_array& pattern) {
	String_array array;
	std::copy_if(base.begin(), base.end(), std::back_inserter(array), [&pattern](const auto& element) {
		return std::find(pattern.begin(), pattern.end(), element) == pattern.end();
	});
	return array;
}

String_array string_to_vector(std::string string) {
	std::replace(string.begin(), string.end(), ',', ' ');
	std::istringstream ss(std::move(string));
	String_array vector;
	for (std::string buffer; ss >> buffer;)
		vector.push_back(std::move(buffer));
	return vector;
}

Log_level decode_log_level(const std::string& string) {
	if (string == "NONE")
		return Log_level::NONE;
	if (string == "CRITICAL")
		return Log_level::CRITICAL;
	if (string == "BASIC")
		return Log_level::BASIC;
	if (string == "WARNING")
		return Log_level::WARNING;
	if (string == "INFO")
		return Log_level::INFO;
	if (string == "ALL")
		return Log_level::ALL;
	return static_cast<Log_level>(-1);
}

template<>
int convert_string(const std::string& string) {
	return std::stoi(string);
}

template<>
long convert_string(const std::string& string) {
	return std::stol(string);
}

template<>
double convert_string(const std::string& string) {
	return std::stod(string);
}

template<>
std::string convert_string(const std::string& string) {
	return string;
}

}
