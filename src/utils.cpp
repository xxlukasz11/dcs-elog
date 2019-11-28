#include "utils.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>
#include <iomanip>

namespace utils {

const char SPACE_SEPARATOR = ' ';
const char COMMA_SEPARATOR = ',';

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

std::string create_date_time_string(time_t time, const std::string& format) {
	tm t_local = *std::localtime(&time);
	std::ostringstream ss;
	ss << std::put_time(&t_local, format.c_str());
	return ss.str();
}

String_array string_to_vector(std::string string) {
	std::replace(string.begin(), string.end(), COMMA_SEPARATOR, SPACE_SEPARATOR);
	std::istringstream ss(std::move(string));
	String_array vector;
	for (std::string buffer; ss >> buffer;)
		vector.push_back(std::move(buffer));
	return vector;
}

Pair_array string_to_pair_array(char pair_separator, std::string string) {
	std::istringstream ss(std::move(string));
	Pair_array pair_array;
	for (std::string pair_string; ss >> pair_string;) {
		auto separator_index = pair_string.find(pair_separator);
		if (separator_index != std::string::npos) {
			auto size = pair_string.size();
			auto first = pair_string.substr(0, separator_index);
			auto second = pair_string.substr(separator_index + 1, size - separator_index);
			pair_array.emplace_back(first, second);
		}
	}
	return pair_array;
}

Attachment_info_array pair_array_to_attachment(const Pair_array& pair_array) {
	Attachment_info_array attachment_array;
	attachment_array.reserve(pair_array.size());
	for (const auto& pair : pair_array) {
		attachment_array.emplace_back(pair.first, pair.second);
	}
	return attachment_array;
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
