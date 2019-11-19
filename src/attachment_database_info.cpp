#include "attachment_database_info.h"

Attachment_database_info::Attachment_database_info(const std::string& name, const std::string& type, const std::string& file_name) :
	Attachment_info(name, type), file_name_(file_name) {
}

const std::string& Attachment_database_info::get_file_name() const {
	return file_name_;
}
