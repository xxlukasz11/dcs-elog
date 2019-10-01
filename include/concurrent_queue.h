#ifndef _CONCURRENT_QUEUE_H_
#define _CONCURRENT_QUEUE_H_

#include <experimental/optional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <deque>
#include "socket.h"
#include "log_item.h"

namespace stdd = std::experimental;

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
	template<typename Representation, typename Duration>
	stdd::optional<Queue_element> try_pop(
		const std::chrono::duration<Representation, Duration>& time_to_wait);
	bool is_empty() const;

private:
	std::mutex mtx_;
	std::condition_variable cond_var_;
	std::deque<Queue_element> queue_;
};

using Socket_queue = Concurrent_queue<Socket>;
using Log_item_queue = Concurrent_queue<Log_item>;

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

template<typename Queue_element>
template<typename Representation, typename Duration>
inline stdd::optional<Queue_element> Concurrent_queue<Queue_element>::try_pop(
	const std::chrono::duration<Representation, Duration>& time_to_wait) {
	std::unique_lock<std::mutex> lock(mtx_);
	auto stop_time = std::chrono::high_resolution_clock::now() + time_to_wait;
	bool popped = cond_var_.wait_until(lock, stop_time, [this]() {
		return !queue_.empty();
	});
	if (!popped) {
		return stdd::nullopt;
	}
	auto element = std::move(queue_.front());
	queue_.pop_front();
	return element;
}

template<typename Queue_element>
inline bool Concurrent_queue<Queue_element>::is_empty() const {
	return queue_.empty();
}

#endif // !_CONCURRENT_QUEUE_H_
