#ifndef _FILE_NAME_PARSER_H_
#define _FILE_NAME_PARSER_H_

#include <string>

class File_name_parser {
public:
	bool parse(const std::string& file_name);
	const std::string& get_name() const;
	const std::string& get_extension() const;

private:
	bool is_valid_file_name(const std::string& file_name);

	std::string name_;
	std::string extension_;
};

#endif // !_FILE_NAME_PARSER_H_
