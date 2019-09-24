#include "elog_errors.h"

const char* elog_decode_error_msg(Elog_error_code error_code) {
	switch (error_code) {
	case ELOG_SUCCESS: return "Event has been saved";
	case ElOG_FAILURE: return "Event has not been saved";
	case ELOG_CREATE_SOCKET_ERROR: return "Could not create socket";
	case ELOG_CONNECT_ERROR: return "Cannot connect to the server";
	case ELOG_CREATE_EVENT_MESSAGE_ERROR: return "Cannot create event message. String may be too long";
	case ELOG_SEND_EVENT_ERROR: return "Error while sending event message";
	default: return "Unknown error code";
	}
}