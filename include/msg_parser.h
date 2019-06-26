/* 
 * valid message structure
 * [mode][size_1 size_2 ... size_n][var 1][var 2]...[var n]
 * mode 0 - insert
 * mode 1 - select
 */

#ifndef _MSG_PARSER_H_
#define _MSG_PARSER_H_

#include <vector>
#include <string>

#include "variadic_exception.h"

class Msg_parser {
public:
	Msg_parser(const std::string& msg);

	enum class mode{
		insert,
		select,
		return_tags_tree,
		add_tag
	};

	bool has_next() const;
	std::string next();
	mode get_mode() const;

	static std::vector<std::string> extract_tags(std::string tags_chunk);

private:
	std::string raw_next();

	mode mode_;
	int chunks_;
	int current_chunk_;
	std::vector<int> chunks_sizes_;

	size_t position_{ 0 };
	std::string msg_;
};

class Mag_parser_exception : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

#endif