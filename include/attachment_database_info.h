#ifndef _ATTACHMENT_DATABASE_INFO_
#define _ATTACHMENT_DATABASE_INFO_

#include <string>
#include <vector>
#include "attachment_info.h"

class Attachment_database_info : public Attachment_info {
public:
	Attachment_database_info(const std::string& name, const std::string& type, const std::string& file_name);

	const std::string& get_file_name() const;

private:
	std::string file_name_;
};

using Attachment_database_info_array = std::vector<Attachment_database_info>;

#endif // !_ATTACHMENT_DATABASE_INFO_
