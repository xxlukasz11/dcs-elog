#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

namespace utils{
	std::string concatenate_string_array(const std::vector<std::string>& array);
	std::vector<std::string> string_to_vector(std::string string);
};

#endif