#include "thread_base.h"

void Thread_base::join() {
	thread_.join();
	thread_is_up_ = false;
}

void Thread_base::start() {
	if (thread_is_up_ == false) {
		thread_is_up_ = true;
		thread_ = Raii_thread{ &Thread_base::run, this };
	}
}
