#ifndef _VARIADIC_EXCEPTION_H_
#define _VARIADIC_EXCEPTION_H_

#include "base_exception.h"
#include <string>
#include <sstream>
#include <utility>

class Variadic_exception : public Base_exception {
public:
	template<typename... T>
	Variadic_exception(T&&... args);
	virtual const std::string& what() const noexcept override;

private:
	std::string message_;
};

template<typename... T>
Variadic_exception::Variadic_exception(T&&... args){
	std::ostringstream stream;
	(int[]){0, ( (stream << args), 0 ) ... };
	message_ = stream.str();
}

#endif