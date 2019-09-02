#ifndef _UPDATE_TAG_REQUEST_H_
#define _UPDATE_TAG_REQUEST_H_

#include "message.h"

class Update_tag_request : public Message {
public:
	Update_tag_request();
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;

	const std::string& get_tag_id() const;
	const std::string& get_new_name() const;

private:
	std::string tag_id_;
	std::string new_name_;
};

#endif