#ifndef _ATTACHMENT_INFO_H_
#define _ATTACHMENT_INFO_H_

#include <string>

class Attachment_info {
public:
	Attachment_info(const std::string& name, const std::string& type);
	const std::string& get_name() const;
	const std::string& get_type() const;

private:
	std::string name_;
	std::string type_;
};

#endif // !_ATTACHMENT_INFO_H_
