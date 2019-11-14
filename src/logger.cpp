#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include "logger.h"

static const std::string COLON_AND_SPACE = ": ";
static const std::string TAB = "\t";
static const std::string DOUBLE_TAB = "\t\t";
static const char* LOG_DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
static const int defult_timeout_value = 10;

Log_item_queue Logger::queue_;

Logger::Logger(const std::string& file_path) : stream_(file_path), timeout_(defult_timeout_value) {
}

void Logger::set_timeout(Timeout_t timeout) {
	timeout_ = timeout;
}

void Logger::set_log_level(Log_level log_level) {
	log_level_ = log_level;
}

Log_event_provider Logger::create() {
	return Log_event_provider(queue_);
}

void Logger::run() {
	release_flag_ = true;
	while (release_flag_ || !queue_.is_empty()) {
		auto log_item = queue_.try_pop(timeout_);
		if (log_item == stdd::nullopt) {
			stream_.close();
		}
		else {
			write_log(log_item.value());
		}
	}
	stream_.flush();
	stream_.close();
}

void Logger::release() {
	release_flag_ = false;
	Logger::create().level(Log_level::INFO).status("Logger released");
}

std::string Logger::create_date_time_string(time_t time) const {
	tm t_local = *std::localtime(&time);
	std::ostringstream ss;
	ss << std::put_time(&t_local, LOG_DATE_FORMAT);
	return ss.str();
}

/*
*  Log location is not already needed
*/
Log_entry Logger::create_log_entry(const Log_item& log_item) {
	std::string date_time_string = create_date_time_string(log_item.get_time());
	std::string entry_type = determine_entry_type(log_item);
	std::string context;
	if (log_item.has_context()) {
		context = std::to_string(log_item.get_context()) + COLON_AND_SPACE;
	}

	Log_entry log_entry;
	log_entry
		.append(std::move(date_time_string))
		.append(TAB)
		.append(entry_type)
		.append(DOUBLE_TAB)
		//.append(log_item.get_location())
		//.append(DOUBLE_TAB)
		.append(context)
		.append(log_item.get_message());
	return log_entry;
}

std::string Logger::determine_entry_type(const Log_item& log_item) {
	using T = Log_item::Type;
	auto item_type = log_item.get_type();

	switch (item_type) {
		case T::INFO: return "INFO";
		case T::WARNING: return "WARNING";
		case T::ERROR: return "ERROR";
		case T::STATUS: return "STATUS";

		default: return "UNKNOWN";
	}
}

bool Logger::should_be_logged(const Log_item& log_item) {
	return log_item.get_log_level() <= log_level_;
}

void Logger::write_log(const Log_item& log_item) {
	if (should_be_logged(log_item)) {
		Log_entry log_entry = create_log_entry(log_item);
		stream_.open_if_closed();
		stream_.write_line(log_entry);
		stream_.flush();
	}
}
