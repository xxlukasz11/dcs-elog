#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include "msg_parser.h"

/*
Base class for every incoming message
*/
class Message {
public:
	enum class Type {
		create_event,
		return_events,
		return_tags_tree,
		create_tag,
		delete_tag,
		update_tag,
		update_event,
		create_library_event
	};

	virtual void extract_parameters(Msg_parser& parser) = 0;
	virtual std::string name() const = 0;
	virtual Type get_message_type() const = 0;

	virtual ~Message() = default;
	static Type int_to_message_type(int mode);
};

#endif