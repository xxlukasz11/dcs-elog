/* 
 * valid message structure
 * [mode][size_1 size_2 ... size_n][var 1][var 2]...[var n]
 */

#ifndef _MSG_PARSER_H_
#define _MSG_PARSER_H_

#include <vector>
#include <string>

#include "variadic_exception.h"

class Msg_parser {
public:
	Msg_parser(const std::string& msg);

	bool has_next() const;
	std::string next();
	int get_mode() const;

private:
	std::string raw_next();

	int mode_;
	int chunks_;
	int current_chunk_;
	std::vector<int> chunks_sizes_;

	size_t position_{ 0 };
	std::string msg_;
};

class Msg_parser_exception : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

#endif