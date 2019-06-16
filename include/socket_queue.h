#ifndef _SOCKET_QUEUE_H_
#define _SOCKET_QUEUE_H_

#include <mutex>
#include <deque>
#include <condition_variable>
#include "socket.h"

class Socket_queue {
	std::mutex mtx_;
	std::condition_variable cond_var_;
	std::deque<Socket> sockets_;

public:
	Socket_queue() = default;
	~Socket_queue() = default;
	Socket_queue(const Socket_queue&) = delete;
	Socket_queue(Socket_queue&&) = delete;
	Socket_queue& operator=(const Socket_queue&) = delete;
	Socket_queue& operator=(Socket_queue&&) = delete;

	void push(Socket socket);
	Socket pop();
};

#endif