#ifndef _LOG_ENTRY_H_
#define _LOG_ENTRY_H_

#include <sstream>
#include <utility>

/*
Part of the logger. Used to build string to be printed in logs.
*/
class Log_entry {
public:
	template<typename T>
	Log_entry& append(T&& element);
	friend std::ostream& operator<<(std::ostream& ostream, const Log_entry& entry);
private:
	std::stringstream log_stream_;
};

std::ostream& operator<<(std::ostream& ostream, const Log_entry& entry);

template<typename T>
inline Log_entry& Log_entry::append(T&& element) {
	log_stream_ << std::forward<T>(element);
	return *this;
}

#endif // !_LOG_BUILDER_H_
