#include "thread_manager.h"

void Thread_manager::add_consumer(const std::shared_ptr<Dcs_thread>& consumer) {
	consumers_.push_back(consumer);
}

void Thread_manager::add_server(const std::shared_ptr<Dcs_thread>& server) {
	servers_.push_back(server);
}

void Thread_manager::start_consumers() {
	for (auto& consumer : consumers_) {
		consumer->start();
	}
}

void Thread_manager::join_consumers(){
	for(auto& consumer : consumers_){
		consumer->join();
	}
}

void Thread_manager::start_servers() {
	for (auto& server : servers_) {
		server->start();
	}
}

void Thread_manager::join_servers(){
	for (auto& server : servers_) {
		server->join();
	}
}
