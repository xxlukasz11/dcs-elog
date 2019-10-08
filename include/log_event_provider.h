#ifndef _LOG_EVENT_PROVIDER_H_
#define _LOG_EVENT_PROVIDER_H_

#include "concurrent_queue.h"
#include "log_item.h"
#include "message.h"
#include "log_level.h"

class Log_event_provider {
public:
	Log_event_provider(Log_item_queue& queue);
	
	Log_event_provider& context(int id);
	Log_event_provider& location(const std::string& location);
	Log_event_provider& level(Log_level level);

	void info(const std::string& message);
	void info(const std::shared_ptr<Message>& message);
	void warning(const std::string& message);
	void error(const std::string& message);
	void status(const std::string& message);

private:
	void push_to_queue();

	Log_item_queue& queue_;
	Log_item log_item_;
};

#endif // !_LOG_EVENT_PROVIDER_H_
