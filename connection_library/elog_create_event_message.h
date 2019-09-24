#ifndef _ELOG_CREATE_EVENT_MESSAGE_H_
#define _ELOG_CREATE_EVENT_MESSAGE_H_

#include "elog_event.h"
#define ELOG_MAX_MESSAGE_LENGTH (ELOG_MAX_STRING_LENGTH*4+100)

typedef struct {
	char data[ELOG_MAX_MESSAGE_LENGTH];
	int length;
} Elog_create_event_message;

Elog_create_event_message create_event_message(const Elog_event* event);

#endif // !_ELOG_CREATE_EVENT_MESSAGE_H_
