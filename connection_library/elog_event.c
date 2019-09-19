#include "string.h"
#include "elog_event.h"

Elog_string create_empty_elog_string() {
	Elog_string elog_string = { .length = 0 };
	memset(elog_string.value, 0, ELOG_STRING_LENGTH);
	return elog_string;
}

Elog_string create_elog_string(const char* value) {
	Elog_string elog_string = create_empty_elog_string();
	if (value) {
		size_t length = strlen(value);
		if (length <= ELOG_STRING_LENGTH) {
			strncpy(elog_string.value, value, length);
			elog_string.length = length;
		}
	}
	return elog_string;
}

Elog_event elog_create_event() {
	Elog_event event = {
		.title = create_empty_elog_string(),
		.description = create_empty_elog_string(),
		.tags = create_empty_elog_string(),
		.author = create_empty_elog_string()
	};
	return event;
}

void elog_set_title(Elog_event* event, const char* title) {
	event->title = create_elog_string(title);
}

void elog_set_description(Elog_event* event, const char* description) {
	event->description = create_elog_string(description);
}

void elog_set_tags(Elog_event* event, const char* tags) {
	event->tags = create_elog_string(tags);
}

void elog_set_author(Elog_event* event, const char* author) {
	event->author = create_elog_string(author);
}
