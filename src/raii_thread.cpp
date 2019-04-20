#include <thread>

#include "raii_thread.h"

void Raii_thread::join(){
	if(thread_.joinable())
		thread_.join();
}

Raii_thread::~Raii_thread() {
	if (thread_.joinable())
		thread_.join();
}