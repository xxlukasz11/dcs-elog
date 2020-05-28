#ifndef _ATTACHMENT_INFO_H_
#define _ATTACHMENT_INFO_H_

#include <string>
#include <vector>

/*
Stores information about attachment name and its MIME type
*/
class Attachment_info {
public:
	Attachment_info(const std::string& name, const std::string& type);
	const std::string& get_name() const;
	const std::string& get_type() const;

private:
	std::string name_;
	std::string type_;
};

using Attachment_info_array = std::vector<Attachment_info>;

#endif // !_ATTACHMENT_INFO_H_
