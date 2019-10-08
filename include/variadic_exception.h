#ifndef _VARIADIC_EXCEPTION_H_
#define _VARIADIC_EXCEPTION_H_

#include "base_exception.h"
#include <string>
#include <sstream>
#include <utility>

template<typename Base, typename Derived>
using is_not_copy_constructor = std::enable_if_t<
	!std::is_base_of<Base, typename std::remove_reference_t<Derived>>::value>;

class Variadic_exception : public Base_exception {
public:
	Variadic_exception() = default;
	template<typename U, typename... T, typename = is_not_copy_constructor<Variadic_exception, U>>
	Variadic_exception(U&& arg, T&&... args);
	virtual const std::string& what() const noexcept override;

private:
	template<typename U, typename... T>
	void expand_parameters(std::ostringstream& stream, U&& arg, T&&... args);
	void expand_parameters(std::ostringstream& stream);

	std::string message_;
};

template<typename U, typename... T, typename>
Variadic_exception::Variadic_exception(U&& arg, T&&... args){
	std::ostringstream stream;
	stream << arg;
	expand_parameters(stream, args...);
}

template<typename U, typename ...T>
inline void Variadic_exception::expand_parameters(std::ostringstream& stream, U&& arg, T && ...args) {
	stream << arg;
	expand_parameters(stream, args...);
}



#endif