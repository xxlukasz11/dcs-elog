#ifndef _RETURN_TAGS_TREE_REQUEST_H_
#define _RETURN_TAGS_TREE_REQUEST_H_

#include "message.h"

class Return_tags_tree_request : public Message {
public:
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;
	virtual Type get_message_type() const;

private:
	
};

#endif