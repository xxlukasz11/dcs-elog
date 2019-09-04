#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <memory>
#include "dcs_thread.h"

class Thread_manager{
public:
	Thread_manager() = default;
	~Thread_manager() = default;
	Thread_manager(Thread_manager&&) = default;
	Thread_manager& operator=(Thread_manager&&) = default;
	
	Thread_manager(const Thread_manager&) = delete;
	Thread_manager& operator=(const Thread_manager&) = delete;

	void add_consumer(const std::shared_ptr<Dcs_thread>& consumer);
	void add_server(const std::shared_ptr<Dcs_thread>& server);
	void start_consumers();
	void join_consumers();
	void start_servers();
	void join_servers();

private:
	std::vector<std::shared_ptr<Dcs_thread>> consumers_;
	std::vector<std::shared_ptr<Dcs_thread>> servers_;
};

#endif