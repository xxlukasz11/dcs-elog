#include <string>
#include <atomic>
#include "custom_exceptions.h"
#include "connection_handler.h"
#include "socket.h"
#include "logger.h"

#include "tcp_server.h"

Tcp_server::Tcp_server(Socket_queue& socket_queue) : queue_(socket_queue) {
}

Tcp_server::~Tcp_server() {
	if (server_is_running_) {
		stop_and_release_consumers();
	}
	close_socket_if_valid();
}

void Tcp_server::set_ip_address(const std::string& ip_address){
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

void Tcp_server::set_accept_delay_ms(int accept_delay_ms) {
	accept_delay_ms_ = accept_delay_ms;
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

int Tcp_server::get_accept_delay_ms() const {
	return accept_delay_ms_;
}

void Tcp_server::initialize(){
	Socket::Addr_in_wrapper addr_in;
	addr_in.set_port(port_);
	if (addr_in.set_ip_address(ip_address_) <= 0) {
		throw Init_server_error("Cannot convert server ip address: ", ip_address_);
	}
    
	server_socket_ = Socket::create(
		Socket::Domain::IPV4,
		Socket::Type::TCP_IP,
		Socket::Protocol::DEFAULT
	);

    if( server_socket_.is_not_valid() ) {
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
	Socket::Addr_in_wrapper addr_in;
	server_is_running_ = true;
	
    while(server_is_running_){
		Socket client_socket = server_socket_.accept(addr_in);
		
		if(client_socket.is_not_valid()){
			if( errno == EWOULDBLOCK ){
				std::this_thread::sleep_for( std::chrono::milliseconds(accept_delay_ms_) );
			}
			else{
				Logger::create().context(client_socket).level(Log_level::WARNING)
					.error("Error while accepting connection");
			}
		}
		else{
			client_socket.set_message_length(message_length_);
			queue_.push(client_socket);
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
