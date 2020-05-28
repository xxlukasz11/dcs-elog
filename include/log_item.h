#ifndef _LOG_ITEM_H_
#define _LOG_ITEM_H_

#include <string>
#include <ctime>
#include "log_level.h"

/*
Part of the logger. Stores information about log to be printed
*/
class Log_item {
public:
	enum class Type { INFO, WARNING, ERROR, STATUS };

	Log_item();
	bool has_context() const;
	bool has_location() const;

	time_t get_time() const;
	int get_context() const;
	const std::string& get_location() const;
	const std::string& get_message() const;
	Type get_type() const;
	Log_level get_log_level() const;

	void set_context(int context);
	void set_location(const std::string& location);
	void set_message(Type type, const std::string& message);
	void set_log_level(Log_level log_level);

private:
	bool context_is_set_{ false };

	Type type_;
	Log_level log_level_{ Log_level::INFO };
	time_t time_;
	int context_{ 0 };
	std::string location_;
	std::string message_;
};

#endif // !_LOG_ITEM_H_
