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

}
