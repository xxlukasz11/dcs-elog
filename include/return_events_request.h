#ifndef _RETURN_EVENTS_REQUEST_H_
#define _RETURN_EVENTS_REQUEST_H_

#include "message.h"

class Return_events_request : public Message {
public:
	virtual void extract_parameters(Msg_parser& parser);
	virtual std::string name() const;
	virtual Type get_message_type() const;

	const std::string& get_min_date() const;
	const std::string& get_max_date() const;
	const std::string& get_tags() const;
private:
	std::string min_date_;
	std::string max_date_;
	std::string tags_;
};

#endif