#include "attachment_info.h"

Attachment_info::Attachment_info(const std::string& name, const std::string& type) :
	name_(name), type_(type) {
}

const std::string& Attachment_info::get_name() const {
	return name_;
}

const std::string& Attachment_info::get_type() const {
	return type_;
}
