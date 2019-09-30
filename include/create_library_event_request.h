#ifndef _CREATE_LIBRARY_EVENT_REQUEST_H_
#define _CREATE_LIBRARY_EVENT_REQUEST_H_

#include "create_event_request.h"
#include <string>

class Create_library_event_request : public Create_event_request {
public:
	virtual std::string name() const;
	virtual Type get_message_type() const;
};

#endif // !_CREATE_LIBRARY_EVENT_REQUEST_H_
