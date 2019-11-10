#ifndef _CREATE_EVENT_REQUEST_H_
#define _CREATE_EVENT_REQUEST_H_

#include "message.h"
#include <vector>

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
	const std::vector<std::string>& get_attachments_names() const;

private:
	std::string title_;
	std::string description_;
	std::string tags_;
	std::string author_;
	std::vector<std::string> attachments_names_;
};

#endif