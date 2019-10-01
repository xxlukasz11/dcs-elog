#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <chrono>
#include "dcs_thread.h"
#include "concurrent_queue.h"
#include "log_event_provider.h"
#include "log_item.h"
#include "log_entry.h"
#include "file_stream.h"

class Logger : public Dcs_thread {
public:
	using Timeout_t = std::chrono::seconds;

	Logger(const std::string& file_name);
	void set_timeout(const Timeout_t& timeout);
	static Log_event_provider create();
	virtual void run() override final;
	void release();

private:
	std::string create_date_time_string(time_t time) const;
	Log_entry create_log_entry(const Log_item& log_item);
	std::string determine_entry_type(const Log_item& log_item);

	bool release_flag_;
	File_stream stream_;
	Timeout_t timeout_;
	static Log_item_queue queue_;
};

#endif // !_LOGGER_H_
