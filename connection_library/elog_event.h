#ifndef _ELOG_EVENT_H_
#define _ELOG_EVENT_H_

typedef struct {
	char* value;
	int length;
} Elog_string;

typedef struct {
	Elog_string title;
	Elog_string description;
	Elog_string tags;
	Elog_string author;
} Elog_event;

#endif // !_ELOG_EVENT_H_
