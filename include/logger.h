#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <chrono>
#include "thread_base.h"
#include "concurrent_queue.h"
#include "log_event_provider.h"
#include "log_item.h"
#include "log_entry.h"
#include "log_level.h"
#include "file_stream.h"

/*
Main class os the logger. Manage log file and writes to it
*/
class Logger : public Thread_base {
public:
	using Timeout_t = std::chrono::seconds;

	Logger(const std::string& file_name);
	void set_timeout(Timeout_t timeout);
	void set_log_level(Log_level log_level);
	static Log_event_provider create();
	virtual void run() override final;
	void release();

private:
	Log_entry create_log_entry(const Log_item& log_item);
	std::string determine_entry_type(const Log_item& log_item);
	bool should_be_logged(const Log_item& log_item);
	void write_log(const Log_item& log_item);

	bool release_flag_;
	File_stream stream_;
	Timeout_t timeout_;
	Log_level log_level_;
	static Log_item_queue queue_;
};

#endif // !_LOGGER_H_
