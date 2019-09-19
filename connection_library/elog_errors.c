#include "elog_errors.h"

const char* elog_decode_error_msg(Elog_error_code error_code) {
	switch (error_code) {
	case ELOG_SUCCESS: return "Success";
	case ELOG_CREATE_SOCKET_ERROR: return "Could not create socket";
	case ELOG_CONNECT_ERROR: return "Cannot connect to the server";
	default: return "Unknown error code";
	}
}