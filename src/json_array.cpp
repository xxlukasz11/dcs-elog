#include <sstream>
#include "json_array.h"

namespace json {

std::string Json_array::to_string() const {
	return SQUARE_BRACE_OPEN + values_to_string() + SQUARE_BRACE_CLOSE;
}

Json_array_ptr Json_array::create() {
	return std::make_shared<Json_array>();
}

void Json_array::push(const Json_value_ptr& value) {
	values_.push_back(value);
}

std::string Json_array::values_to_string() const {
	std::ostringstream stream;
	size_t no_of_fields = values_.size();
	for (size_t i = 0; i < no_of_fields; ++i) {
		stream << values_[i]->to_string();
		if (i != no_of_fields - 1) {
			stream << COMMA;
		}
	}
	return stream.str();
}

}
