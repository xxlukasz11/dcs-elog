#include <string>
#include <atomic>
#include "utils.h"
#include "custom_exceptions.h"
#include "connection_handler.h"
#include "socket.h"

#include "tcp_server.h"

Tcp_server::Tcp_server(Socket_queue& socket_queue) : queue_(socket_queue) {
}

Tcp_server::~Tcp_server() {
	if (server_is_running_) {
		stop_and_release_consumers();
	}
	close_socket_if_valid();
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

void Tcp_server::set_recieve_timeout_seconds(int seconds){
	recieve_timeout_seconds_ = seconds;
}

void Tcp_server::set_number_of_consumers(int number_of_consumers){
	number_of_consumers_ = number_of_consumers;
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

int Tcp_server::get_recieve_timeout_seconds() const {
	return recieve_timeout_seconds_;
}

int Tcp_server::get_number_of_consumers() const {
	return number_of_consumers_;
}

void Tcp_server::initialize(){
	Socket::Addr_in_wrapper addr_in = Socket::Addr_in_wrapper(port_);
	if (addr_in.set_ip_address(ip_address_) <= 0) {
		throw Init_server_error("Cannot convert server ip address: ", ip_address_);
	}
    
	server_socket_ = Socket::create(
		Socket::Domain::IPV4,
		Socket::Type::TCP_IP,
		Socket::Protocol::DEFAULT
	);

    if( server_socket_.is_invalid() ) {
        throw Init_server_error("Cannot create server socket");
    }

	if( !server_socket_.set_sock_opt(SOL_SOCKET, SO_REUSEADDR) < 0 ){
		throw Init_server_error("Setsockopt with SO_REUSEADDR parameter error");
	}
	
    if( server_socket_.bind(addr_in) < 0 ) {
        throw Init_server_error("Cannot bind server socket");
    }
    
	if( server_socket_.set_to_nonblock_mode() < 0 ){
		throw Init_server_error("Cannot set server socket to NONBLOCK mode");
	}
	
    if( server_socket_.listen(Tcp_server::max_connections_) < 0 ) {
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

void Tcp_server::close_socket_if_valid() {
	if (server_socket_.is_valid()) {
		server_socket_.shutdown_rdwr();
	}
}

void Tcp_server::stop_and_release_consumers(){
	close_socket_if_valid();
	server_is_running_ = false;
	for(int i = 0; i < number_of_consumers_; ++i)
		queue_.push(Tcp_server::DUMMY_SOCKET_);
}
