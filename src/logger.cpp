#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include "logger.h"

static const std::string TAB = "\t";
Log_item_queue Logger::queue_;

Log_event_provider Logger::create() {
	return Log_event_provider(queue_);
}

void Logger::run() {
	while (true) {
		Log_item log_item = queue_.pop();
		Log_entry log_entry = create_log_entry(log_item);
		std::cout << log_entry << std::endl;
	}
}

std::string Logger::create_date_time_string(time_t time) const {
	tm t_local = *std::localtime(&time);
	std::ostringstream ss;
	ss << std::put_time(&t_local, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

Log_entry Logger::create_log_entry(const Log_item& log_item) {
	std::string date_time_string = create_date_time_string(log_item.get_time());
	std::string context;
	if (log_item.has_context()) {
		context = std::to_string(log_item.get_context());
	}

	Log_entry log_entry;
	log_entry
		.append(std::move(date_time_string))
		.append(TAB)
		.append(context)
		.append(":" + TAB)
		.append(log_item.get_location())
		.append(TAB)
		.append(log_item.get_message());
	return log_entry;
}
