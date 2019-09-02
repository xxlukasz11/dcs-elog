#ifndef _UPDATE_EVENT_REQUEST_H_
#define _UPDATE_EVENT_REQUEST_H_

#include "message.h"

class Update_event_request : public Message {
public:
	Update_event_request();
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;

	const std::string& get_event_id() const;
	const std::string& get_title() const;
	const std::string& get_description() const;
	const std::string& get_tags() const;

private:
	std::string event_id_;
	std::string title_;
	std::string description_;
	std::string tags_;
};

#endif