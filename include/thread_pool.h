#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>

#include "consumer.h"
#include "raii_thread.h"

class Thread_pool{
public:
	Thread_pool() = default;
	~Thread_pool() = default;
	Thread_pool(Thread_pool&&) = default;
	Thread_pool& operator=(Thread_pool&&) = default;
	
	Thread_pool(const Thread_pool&) = delete;
	Thread_pool& operator=(const Thread_pool&) = delete;

	void add_consumer(Consumer&& consumer);
	void set_server_thread(Raii_thread&& server_thread);
	void join_consumers();
	void join_server();

private:
	std::vector<Consumer> consumers_;
	Raii_thread server_;
};

#endif