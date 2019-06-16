#include <mutex>

#include "socket_queue.h"

void Socket_queue::push(Socket socket) {
	std::unique_lock<std::mutex> lock(mtx_);
	sockets_.push_back(socket);
	lock.unlock();
	cond_var_.notify_one();
}

Socket Socket_queue::pop() {
	std::unique_lock<std::mutex> lock(mtx_);
	
	cond_var_.wait(lock, [this](){
		return !sockets_.empty();
	});

	auto el = sockets_.front();
	sockets_.pop_front();
	return el;
}