#ifndef _CREATE_EVENT_REQUEST_H_
#define _CREATE_EVENT_REQUEST_H_

#include "message.h"

class Create_event_request : public Message {
public:
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;
	virtual Type get_message_type() const;
	virtual ~Create_event_request() = default;

	const std::string& get_title() const;
	const std::string& get_description() const;
	const std::string& get_tags() const;
	const std::string& get_author() const;

private:
	std::string title_;
	std::string description_;
	std::string tags_;
	std::string author_;
};

#endif