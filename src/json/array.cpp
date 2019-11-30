#include <sstream>
#include "json/array.h"

namespace json {

std::string Array::to_string() const {
	return SQUARE_BRACE_OPEN + values_to_string() + SQUARE_BRACE_CLOSE;
}

Array_ptr Array::create() {
	return std::make_shared<Array>();
}

void Array::push(const Value_ptr& value) {
	values_.push_back(value);
}

size_t Array::size() const {
	return values_.size();
}

Value_ptr& Array::get(size_t index) {
	return values_[index];
}

const Value_ptr& Array::get(size_t index) const {
	return values_[index];
}

std::string Array::values_to_string() const {
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
