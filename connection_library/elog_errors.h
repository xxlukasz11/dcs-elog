typedef enum {
	ELOG_SUCCESS,
	ElOG_FAILURE,
	ELOG_CREATE_SOCKET_ERROR,
	ELOG_CONNECT_ERROR,
	ELOG_CREATE_EVENT_MESSAGE_ERROR,
	ELOG_SEND_EVENT_ERROR
} Elog_error_code;

const char* elog_decode_error_msg(Elog_error_code error_code);