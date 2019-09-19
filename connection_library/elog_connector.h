#ifndef _ELOG_CONNECTOR_H_
#define _ELOG_CONNECTOR_H_

#include "elog_errors.h"
#include "elog_event.h"

const char* elog_ip();
int elog_port();
Elog_error_code send_event(Elog_event* event);

#endif // !_ELOG_CONNECTOR_H_
