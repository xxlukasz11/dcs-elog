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
	void set_server_thread(const std::shared_ptr<Dcs_thread>& server_thread);
	void start_consumers();
	void join_consumers();
	void start_server();
	void join_server();

	const std::shared_ptr<Dcs_thread>& get_server_thread() const;

private:
	std::vector<std::shared_ptr<Dcs_thread>> consumers_;
	std::shared_ptr<Dcs_thread> server_;
};

#endif