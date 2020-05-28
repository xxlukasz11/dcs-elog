#ifndef _CREATE_TAG_REQUEST_H_
#define _CREATE_TAG_REQUEST_H_

#include "message.h"

/*
Message used to trigger create_tag_procedure
*/
class Create_tag_request : public Message {
public:
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;
	virtual Type get_message_type() const;

	const std::string& get_tag_name() const;
	const std::string& get_parent_id() const;

private:
	std::string tag_name_;
	std::string parent_id_;
};

#endif