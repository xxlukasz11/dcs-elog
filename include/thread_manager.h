#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <memory>
#include "dcs_thread.h"

class Thread_manager{
	using Thread_element = std::shared_ptr<Dcs_thread>;
	using Thread_list = std::vector<Thread_element>;
public:
	Thread_manager() = default;
	~Thread_manager() = default;
	Thread_manager(Thread_manager&&) = default;
	Thread_manager& operator=(Thread_manager&&) = default;
	
	Thread_manager(const Thread_manager&) = delete;
	Thread_manager& operator=(const Thread_manager&) = delete;

	void add_consumer(const Thread_element& consumer);
	void add_server(const Thread_element& server);
	void add_logger(const Thread_element& logger);
	void start_consumers();
	void join_consumers();
	void start_servers();
	void join_servers();
	void start_loggers();
	void join_loggers();
private:
	void start_threads(Thread_list& threads);
	void join_threads(Thread_list& threads);

	Thread_list consumers_;
	Thread_list servers_;
	Thread_list loggers_;
};

#endif