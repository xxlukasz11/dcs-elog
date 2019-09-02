#ifndef _DELETE_TAG_REQUEST_H_
#define _DELETE_TAG_REQUEST_H_

#include "message.h"

class Delete_tag_request : public Message {
public:
	Delete_tag_request();
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;

	const std::string& get_tag_id() const;

private:
	std::string tag_id_;
};

#endif