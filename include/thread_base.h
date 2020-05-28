#ifndef _THREAD_BASE_H_
#define _THREAD_BASE_H_

#include <atomic>
#include "raii_thread.h"

/*
Base class for every class which is supposed to run on separate thread
*/
class Thread_base {
public:
	void join();
	void start();
	virtual void run() = 0;
	virtual ~Thread_base() = default;
private:
	Raii_thread thread_;
	std::atomic<bool> thread_is_up_{ false };
};

#endif // !_THREAD_BASE_H_
