#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "elog_connector.h"
#include "elog_create_event_message.h"

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
		*error_code = ELOG_CREATE_SOCKET_ERROR;
		return -1;
	}
	if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		*error_code = ELOG_CONNECT_ERROR;
		return -1;
	}
	return sockfd;
}

void send_message_contents(int sockfd, Elog_error_code* error_code, Elog_create_event_message* message) {
	int length = message->length;
	if (write(sockfd, &length, sizeof(length)) <= 0) {
		*error_code = ELOG_SEND_EVENT_ERROR;
		return;
	}
	if (write(sockfd, message->data, sizeof(message->data)) <= 0) {
		*error_code = ELOG_SEND_EVENT_ERROR;
		return;
	}
}

void receive_server_response(int sockfd, Elog_error_code* error_code) {
	char recv_buffer[ELOG_MAX_STRING_LENGTH];
	memset(recv_buffer, 0, ELOG_MAX_STRING_LENGTH);
	read(sockfd, recv_buffer, sizeof(recv_buffer));
	if (strstr(recv_buffer, "successfully") == NULL) {
		*error_code = ElOG_FAILURE;
	}
}

void send_data(int sockfd, const Elog_event* event, Elog_error_code* error_code) {
	Elog_create_event_message* message = create_event_message(event);
	send_message_contents(sockfd, error_code, message);
	if (*error_code == ELOG_SUCCESS) {
		receive_server_response(sockfd, error_code);
	}
	free_event_message(message);
}

void close_connection(int sockfd) {
	close(sockfd);
}

Elog_error_code send_event(const Elog_event* event) {
	Elog_error_code error_code = ELOG_SUCCESS;
	int sockfd = open_connection(&error_code);
	if (error_code != ELOG_SUCCESS) {
		return error_code;
	}
	send_data(sockfd, event, &error_code);
	close_connection(sockfd);
	return error_code;
}
