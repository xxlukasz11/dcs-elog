#ifndef _ELOG_EVENT_H_
#define _ELOG_EVENT_H_

#define ELOG_MAX_STRING_LENGTH 512

#include <stdlib.h>

typedef struct {
	char value[ELOG_MAX_STRING_LENGTH];
	size_t length;
} Elog_string;

typedef struct {
	Elog_string title;
	Elog_string description;
	Elog_string tags;
	Elog_string author;
} Elog_event;

Elog_event elog_create_event();
void elog_set_title(Elog_event* event, const char* title);
void elog_set_description(Elog_event* event, const char* description);
void elog_set_tags(Elog_event* event, const char* tags);
void elog_set_author(Elog_event* event, const char* author);

#endif // !_ELOG_EVENT_H_
