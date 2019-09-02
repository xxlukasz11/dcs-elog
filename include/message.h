#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include "msg_parser.h"

class Message {
public:
	enum class Type {
		create_event,
		return_events,
		return_tags_tree,
		create_tag,
		delete_tag,
		update_tag,
		update_event
	};

	virtual void extract_parameters(Msg_parser& parser) = 0;
	virtual std::string name() const = 0;

	Message(Type message_type);
	Type get_message_type() const;
	virtual ~Message() = default;
	static Type int_to_message_type(int mode);
private:
	Type message_type_;
};

#endif