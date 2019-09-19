typedef enum {
	ELOG_SUCCESS,
	ELOG_CREATE_SOCKET_ERROR,
	ELOG_CONNECT_ERROR
} Elog_error_code;

const char* elog_decode_error_msg(Elog_error_code error_code);