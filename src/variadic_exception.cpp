#include "variadic_exception.h"

const std::string& Variadic_exception::what() const noexcept {
	return message_;
}

void Variadic_exception::expand_parameters(std::ostringstream& stream) {
	message_ = stream.str();
}
