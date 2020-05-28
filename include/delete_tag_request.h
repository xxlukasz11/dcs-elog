#ifndef _DELETE_TAG_REQUEST_H_
#define _DELETE_TAG_REQUEST_H_

#include "message.h"

/*
Message used to trigger delete_tag_procedure
*/
class Delete_tag_request : public Message {
public:
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;
	virtual Type get_message_type() const;

	const std::string& get_tag_id() const;

private:
	std::string tag_id_;
};

#endif