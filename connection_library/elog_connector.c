#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include "elog_connector.h"

const char* elog_decode_error_msg(Elog_error_code error_code) {
	switch (error_code) {
	case ELOG_SUCCESS: return "Success";
	case ELOG_CREATE_SOCKET: return "Could not create socket";
	case ELOG_CONNECT: return "Cannot connect to the server";
	default: return "Unknown error code";
	}
}

const char* elog_ip() {
	return "127.0.0.1";
}

int elog_port() {
	return 9100;
}

int open_connection(Elog_error_code* error_code) {
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_addr = inet_addr(elog_ip()),
		.sin_port = htons(elog_port())
	};

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		*error_code = ELOG_CREATE_SOCKET;
		return -1;
	}
	if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		*error_code = ELOG_CONNECT;
		return -1;
	}
	return sockfd;
}

void send_data(int socket, Elog_event event, Elog_error_code* error_code) {

}

void close_connection(int sockfd) {
	close(sockfd);
}

Elog_error_code send_event(Elog_event event) {
	Elog_error_code error_code = ELOG_SUCCESS;

	int sockfd = open_connection(&error_code);
	if(sockfd != ELOG_SUCCESS)
		return error_code;

	send_data(sockfd, event, error_code);
	close_connection(sockfd);
	return error_code;
}
