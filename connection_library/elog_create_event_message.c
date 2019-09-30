#include <string.h>
#include <stdio.h>
#include "elog_create_event_message.h"

enum MsgType {
	create_event,
	return_events,
	return_tags_tree,
	create_tag,
	delete_tag,
	update_tag,
	update_event,
	create_library_event
};

void open_section(Elog_create_event_message* message) {
	strcat(message->data, "[");
	message->length += 1;
}

void close_section(Elog_create_event_message* message) {
	strcat(message->data, "]");
	message->length += 1;
}

void append_number(Elog_create_event_message* message, int number) {
	char number_string[12];
	sprintf(number_string, "%d", number);
	int string_length = strlen(number_string);
	strncat(message->data, number_string, string_length);
	message->length += string_length;
}

void append_string(Elog_create_event_message* message, const Elog_string* elog_string) {
	strncat(message->data, elog_string->value, elog_string->length);
	message->length += elog_string->length;
}

void append_string_length(Elog_create_event_message* message, const Elog_string* elog_string) {
	append_number(message, elog_string->length);
}

void append_raw_string(Elog_create_event_message* message, const char* str) {
	strcat(message->data, str);
	message->length += strlen(str);
}

void append_space(Elog_create_event_message* message) {
	strcat(message->data, " ");
	message->length += 1;
}


void append_mode_section(Elog_create_event_message* message, int mode) {
	open_section(message);
	append_number(message, mode);
	close_section(message);
}


void append_header_section(Elog_create_event_message* message, const Elog_event* event) {
	open_section(message);
	append_string_length(message, &(event->title));
	append_space(message);
	append_string_length(message, &(event->description));
	append_space(message);
	append_string_length(message, &(event->tags));
	append_space(message);
	append_string_length(message, &(event->author));
	close_section(message);
}

void append_body_sections(Elog_create_event_message* message, const Elog_event* event) {
	open_section(message);
	append_string(message, &(event->title));
	close_section(message);

	open_section(message);
	append_string(message, &(event->description));
	close_section(message);

	open_section(message);
	append_string(message, &(event->tags));
	close_section(message);

	open_section(message);
	append_string(message, &(event->author));
	close_section(message);
}

void set_message_data(Elog_create_event_message* message, const Elog_event* event) {
	append_mode_section(message, create_library_event);
	append_header_section(message, event);
	append_body_sections(message, event);
}

Elog_create_event_message* create_event_message(const Elog_event* event) {
	Elog_create_event_message* message = (Elog_create_event_message*)malloc(sizeof(Elog_create_event_message));
	message->length = 0;
	memset(message->data, 0, ELOG_MAX_MESSAGE_LENGTH);
	set_message_data(message, event);
	return message;
}

void free_event_message(Elog_create_event_message* message) {
	free(message);
}
