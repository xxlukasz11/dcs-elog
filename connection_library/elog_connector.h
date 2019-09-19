#ifndef _ELOG_CONNECTOR_H_
#define _ELOG_CONNECTOR_H_

#include "elog_event.h"

typedef enum {
	ELOG_SUCCESS,
	ELOG_CREATE_SOCKET,
	ELOG_CONNECT
} Elog_error_code;

const char* elog_decode_error_msg(Elog_error_code error_code);
const char* elog_ip();
int elog_port();
Elog_error_code send_event(Elog_event event);

#endif // !_ELOG_CONNECTOR_H_
