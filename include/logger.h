#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "dcs_thread.h"
#include "concurrent_queue.h"
#include "log_event_provider.h"
#include "log_item.h"
#include "log_entry.h"

class Logger : public Dcs_thread {
public:
	static Log_event_provider create();
	virtual void run() override final;
private:
	std::string create_date_time_string(time_t time) const;
	Log_entry create_log_entry(const Log_item& log_item);

	static Log_item_queue queue_;
};

#endif // !_LOGGER_H_
