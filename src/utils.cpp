#include "utils.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>

std::string utils::concatenate_string_array(const std::vector<std::string>& array) {
	std::string text;
	size_t array_size = array.size();
	for (size_t i = 0; i < array_size; ++i) {
		text += array[i];
		if (i != array_size - 1)
			text += ", ";
	}
	return text;
}

std::vector<std::string> utils::string_to_vector(std::string string) {
	std::replace(string.begin(), string.end(), ',', ' ');
	std::istringstream ss(std::move(string));
	std::vector<std::string> vector;
	for (std::string buffer; ss >> buffer;)
		vector.push_back(std::move(buffer));
	return vector;
}
