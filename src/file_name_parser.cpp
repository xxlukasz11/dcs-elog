#include "file_name_parser.h"

bool File_name_parser::parse(const std::string& file_name) {
	if (!is_valid_file_name(file_name)) {
		name_ = "";
		extension_ = "";
		return false;
	}

	const auto last_dot_index = file_name.rfind('.');
	const auto last_slash_index = file_name.rfind('/');
	bool has_extension = last_dot_index != std::string::npos
		&& (last_slash_index == std::string::npos || last_slash_index < last_dot_index);

	size_t file_name_start = 0;
	if (last_slash_index != std::string::npos) {
		file_name_start = last_slash_index + 1;
	}

	if (has_extension) {
		name_ = file_name.substr(file_name_start, last_dot_index - file_name_start);
		extension_ = file_name.substr(last_dot_index);
	}
	else {
		name_ = file_name.substr(file_name_start);
		extension_ = "";
	}
	
	return true;
}

const std::string& File_name_parser::get_name() const {
	return name_;
}

const std::string& File_name_parser::get_extension() const {
	return extension_;
}

bool File_name_parser::is_valid_file_name(const std::string& file_name) {
	return file_name.size() > 0
		&& file_name[file_name.size() - 1] != '/'
		&& file_name[file_name.size() - 1] != '.';
}
