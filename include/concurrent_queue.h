#ifndef _MULTITHREAD_QUEUE_H_
#define _MULTITHREAD_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <deque>
#include "socket.h"

template<typename Queue_element>
class Concurrent_queue {
public:
	Concurrent_queue() = default;
	~Concurrent_queue() = default;
	Concurrent_queue(const Concurrent_queue&) = delete;
	Concurrent_queue(Concurrent_queue&&) = delete;
	Concurrent_queue& operator=(const Concurrent_queue&) = delete;
	Concurrent_queue& operator=(Concurrent_queue&&) = delete;

	void push(const Queue_element& element);
	void push(Queue_element&& element);
	Queue_element pop();

private:
	std::mutex mtx_;
	std::condition_variable cond_var_;
	std::deque<Queue_element> queue_;
};

using Socket_queue = Concurrent_queue<Socket>;

template<typename Queue_element>
inline void Concurrent_queue<Queue_element>::push(const Queue_element& element) {
	std::unique_lock<std::mutex> lock(mtx_);
	queue_.push_back(element);
	lock.unlock();
	cond_var_.notify_one();
}

template<typename Queue_element>
inline void Concurrent_queue<Queue_element>::push(Queue_element&& element) {
	std::unique_lock<std::mutex> lock(mtx_);
	queue_.push_back(std::move(element));
	lock.unlock();
	cond_var_.notify_one();
}

template<typename Queue_element>
inline Queue_element Concurrent_queue<Queue_element>::pop() {
	std::unique_lock<std::mutex> lock(mtx_);
	cond_var_.wait(lock, [this]() {
		return !queue_.empty();
	});
	auto element = std::move(queue_.front());
	queue_.pop_front();
	return element;
}

#endif // !_MULTITHREAD_QUEUE_H_
