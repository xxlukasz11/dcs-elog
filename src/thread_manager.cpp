#include "thread_manager.h"

void Thread_manager::add_consumer(const std::shared_ptr<Dcs_thread>& consumer) {
	consumers_.push_back(consumer);
}

void Thread_manager::set_server_thread(const std::shared_ptr<Dcs_thread>& server_thread){
	server_ = server_thread;
}

void Thread_manager::start_consumers() {
	for (auto& t : consumers_) {
		t->start();
	}
}

void Thread_manager::join_consumers(){
	for(auto& t : consumers_){
		t->join();
	}
}

void Thread_manager::start_server() {
	server_->start();
}

void Thread_manager::join_server(){
	server_->join();
}

const std::shared_ptr<Dcs_thread>& Thread_manager::get_server_thread() const {
	return server_;
}

