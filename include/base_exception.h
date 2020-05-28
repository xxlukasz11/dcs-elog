#ifndef _BASE_EXCEPTION_H_
#define _BASE_EXCEPTION_H_

#include <string>

/*
Base class for exceptions in E-Log
*/
class Base_exception {
public:
	virtual ~Base_exception() = default;
	virtual const std::string& what() const noexcept = 0;
};

#endif