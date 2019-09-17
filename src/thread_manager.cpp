#include "thread_manager.h"

void Thread_manager::add_consumer(const Thread_element& consumer) {
	consumers_.push_back(consumer);
}

void Thread_manager::add_server(const Thread_element& server) {
	servers_.push_back(server);
}

void Thread_manager::add_logger(const Thread_element& logger) {
	loggers_.push_back(logger);
}

void Thread_manager::start_consumers() {
	start_threads(consumers_);
}

void Thread_manager::join_consumers(){
	join_threads(consumers_);
}

void Thread_manager::start_servers() {
	start_threads(servers_);
}

void Thread_manager::join_servers(){
	join_threads(servers_);
}

void Thread_manager::start_loggers() {
	start_threads(loggers_);
}

void Thread_manager::join_loggers() {
	join_threads(loggers_);
}

void Thread_manager::start_threads(Thread_list& thread_list) {
	for (auto& thread : thread_list) {
		thread->start();
	}
}

void Thread_manager::join_threads(Thread_list& thread_list) {
	for (auto& thread : thread_list) {
		thread->join();
	}
}
