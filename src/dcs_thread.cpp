#include "dcs_thread.h"

void Dcs_thread::join() {
	thread_.join();
	thread_is_up_ = false;
}

void Dcs_thread::start() {
	if (thread_is_up_ == false) {
		thread_is_up_ = true;
		thread_ = Raii_thread{ &Dcs_thread::run, this };
	}
}
