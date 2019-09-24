#include "elog_connector.h"

#include <stdio.h>

int main(int argc, char** argv) {
	Elog_event event = elog_create_event();
	elog_set_title(&event, "title main 2");
	elog_set_description(&event, "description main 2");
	elog_set_tags(&event, "-");
	elog_set_author(&event, "author main 2");

	Elog_error_code error_code = send_event(&event);
	printf("%s\n", elog_decode_error_msg(error_code));
	return 0;
}