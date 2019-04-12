#ifndef _RAII_THREAD_H_
#define _RAII_THREAD_H_

#include <utility>
#include <thread>

class Raii_thread {
public:

	template<typename ...T>
	Raii_thread(T ...args);
	~Raii_thread();

	Raii_thread(const Raii_thread&) = delete;
	Raii_thread(Raii_thread&&) = default;

	Raii_thread& operator=(const Raii_thread&) = delete;
	Raii_thread& operator=(Raii_thread&&) = default;

	void join();

private:
	std::thread thread_;
};

template<typename ...T>
Raii_thread::Raii_thread(T ...args) : thread_( std::forward<T>(args)... ) {}

#endif