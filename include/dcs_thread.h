#ifndef _DCS_THREAD_H_
#define _DCS_THREAD_H_

#include <atomic>
#include "raii_thread.h"

class Dcs_thread {
public:
	void join();
	void start();
	virtual void run() = 0;
	virtual ~Dcs_thread() = default;
private:
	Raii_thread thread_;
	std::atomic<bool> thread_is_up_{ false };
};

#endif // !_DCS_THREAD_H_
