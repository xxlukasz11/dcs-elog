#include <string>
#include <thread>
#include <atomic>
#include <csignal>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/fcntl.h>

#include "utils.h"
#include "custom_exceptions.h"
#include "connection_handler.h"
#include "raii_thread.h"

#include "tcp_server.h"

std::atomic<bool> Tcp_server::server_is_running_{ false };
std::string Tcp_server::ip_address_{ "0.0.0.0" };
int Tcp_server::port_ = { 9100 };
int Tcp_server::message_length_{ 1024 };
int Tcp_server::max_connections_{ 50 };
int Tcp_server::timeout_seconds_{ 5 };
int Tcp_server::number_of_consumers_{ 3 };

Tcp_server::Tcp_server() {
	signal(SIGINT, &Tcp_server::set_stop_variable);
}

Tcp_server::~Tcp_server(){
	if(server_is_running_){
		set_stop_variable(int{});
		join_threads();
	}

	if(server_socket_ > 0)
		shutdown(server_socket_, SHUT_RDWR);
}

Tcp_server& Tcp_server::get_instance(){
	static Tcp_server instance;
	return instance;
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

int Tcp_server::get_message_length() {
	return message_length_;
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

void Tcp_server::start_server(){
	Tcp_server::server_is_running_ = true;

	for(int i = 0; i < number_of_consumers_; ++i)
		threads_manager_.add_consumer( Connection_handler(queue_) );
	
	threads_manager_.set_server_thread( Raii_thread(&Tcp_server::run_server, this) );
}

void Tcp_server::join_threads(){
	threads_manager_.join_server();
	release_consumers();
	threads_manager_.join_consumers();
}

void Tcp_server::run_server(){
	struct sockaddr_in client;
    socklen_t len = sizeof(client);
	
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
			queue_.push(client_socket);
		}
    }
}

void Tcp_server::release_consumers(){
	for(int i = 0; i < number_of_consumers_; ++i)
		queue_.push(Tcp_server::DUMMY_SOCKET_);
}

void Tcp_server::set_stop_variable(int){
	Tcp_server::server_is_running_ = false;
}
