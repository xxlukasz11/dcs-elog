#include "variadic_exception.h"

const std::string& Variadic_exception::what() const noexcept {
	return message_;
}