#include <string>
#include <atomic>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/fcntl.h>

#include "utils.h"
#include "custom_exceptions.h"
#include "connection_handler.h"
#include "socket.h"

#include "tcp_server.h"

Tcp_server::Tcp_server(Socket_queue& socket_queue) : queue_(socket_queue) {
}

Tcp_server::~Tcp_server(){
	if(server_is_running_){
		release_consumers();
	}

	if(server_socket_ > 0)
		shutdown(server_socket_, SHUT_RDWR);
}

void Tcp_server::set_ip_address(std::string ip_address){
	ip_address_ = ip_address;
}

void Tcp_server::set_port(int port){
	port_ = port;
}

void Tcp_server::set_message_length(int length){
	message_length_ = length;
}

void Tcp_server::set_max_connections(int max_connections){
	max_connections_ = max_connections;
}

void Tcp_server::set_timeout_seconds(int seconds){
	timeout_seconds_ = seconds;
}

void Tcp_server::set_number_of_consumers(int n_consumers){
	number_of_consumers_ = n_consumers;
}

const std::atomic<bool>& Tcp_server::get_running_flag() const {
	return server_is_running_;
}

std::string Tcp_server::get_ip_address() const {
	return ip_address_;
}

int Tcp_server::get_port() const {
	return port_;
}

Socket_queue & Tcp_server::get_socket_queue() {
	return queue_;
}

int Tcp_server::get_message_length() const {
	return message_length_;
}

int Tcp_server::get_max_connections() const {
	return max_connections_;
}

int Tcp_server::get_timeout_seconds() const {
	return timeout_seconds_;
}

int Tcp_server::get_number_of_consumers() const {
	return number_of_consumers_;
}

void Tcp_server::initialize(){

	struct sockaddr_in server_config = {
        .sin_family = AF_INET,
        .sin_port = htons(port_)
    };
    
    if( inet_pton(server_config.sin_family, ip_address_.c_str(), &server_config.sin_addr) <= 0 ) {
        throw Init_server_error("Cannot convert server ip address: ", ip_address_);
    }
    
    server_socket_ = socket( AF_INET, SOCK_STREAM, 0);
    if( server_socket_ < 0 ) {
        throw Init_server_error("Cannot create server socket");
    }
    
	int opt = 1;
	if( setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ){
		throw Init_server_error("Setsockopt with SO_REUSEADDR parameter error");
	}
	
    if( bind(server_socket_, (struct sockaddr*) &server_config, sizeof(server_config) ) < 0 ) {
        throw Init_server_error("Cannot bind server socket");
    }
    
	if( fcntl(server_socket_, F_SETFL, O_NONBLOCK) < 0 ){
		throw Init_server_error("Cannot set server socket to NONBLOCK mode");
	}
	
    if( listen(server_socket_, Tcp_server::max_connections_) < 0 ) {
        throw Init_server_error("Error: Cant listen to the port ", port_);
    }

}

void Tcp_server::run(){
	struct sockaddr_in client;
    socklen_t len = sizeof(client);
	server_is_running_ = true;
	
    while(server_is_running_){
		int client_socket = accept( server_socket_, (struct sockaddr *) &client, &len );
		
		if(client_socket < 0){
			if( errno == EWOULDBLOCK ){
				std::this_thread::sleep_for( std::chrono::milliseconds(Tcp_server::ACCEPT_DELAY_MS) );
			}
			else{
				utils::err_log(client_socket, ": Error while accepting connection");
			}
		}
		else{
			Socket socket_wrapper(client_socket);
			socket_wrapper.set_message_length(message_length_);
			queue_.push(socket_wrapper);
		}
    }
}

void Tcp_server::release_consumers(){
	server_is_running_ = false;
	for(int i = 0; i < number_of_consumers_; ++i)
		queue_.push(Tcp_server::DUMMY_SOCKET_);
}
