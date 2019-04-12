#include <utility>

#include "thread_pool.h"

void Thread_pool::add_consumer(Consumer&& consumer){
	consumers_.push_back( std::move(consumer) );
}

void Thread_pool::set_server_thread(Raii_thread&& server_thread){
	server_ = std::move(server_thread);
}

void Thread_pool::join_consumers(){
	for(auto& t : consumers_){
		t.join();
	}
}

void Thread_pool::join_server(){
	server_.join();
}

