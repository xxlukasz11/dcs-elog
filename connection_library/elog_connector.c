#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "string.h"
#include "elog_connector.h"

#include <stdio.h>

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

void send_data(int sockfd, Elog_event* event, Elog_error_code* error_code) {
	char send_buffer[] = "[0][7 6 3 8][title A][desc B][d,g][komputer]";
	int length = strlen(send_buffer);
	write(sockfd, &length, sizeof(length));
	write(sockfd, send_buffer, sizeof(send_buffer));

	char recv_buffer[ELOG_STRING_LENGTH];
	read(sockfd, recv_buffer, sizeof(recv_buffer));
	printf("%s\n", recv_buffer);
}

void close_connection(int sockfd) {
	close(sockfd);
}

Elog_error_code send_event(Elog_event* event) {
	Elog_error_code error_code = ELOG_SUCCESS;

	int sockfd = open_connection(&error_code);
	if(error_code != ELOG_SUCCESS)
		return error_code;

	send_data(sockfd, event, &error_code);
	close_connection(sockfd);
	return error_code;
}
