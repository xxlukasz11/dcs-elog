#ifndef _LOG_ITEM_H_
#define _LOG_ITEM_H_

#include <string>
#include <ctime>
#include <experimental/optional>

// C++14 workaround
// TODO delete after migration to C++17
namespace stdd = std::experimental;

class Log_item {
public:
	enum class Type { INFO, WARNING, ERROR };

	Log_item();
	bool has_context() const;
	bool has_location() const;

	time_t get_time() const;
	int get_context() const;
	const std::string& get_location() const;
	const std::string& get_message() const;
	Type get_type() const;

	void set_context(int context);
	void set_location(const std::string& location);
	void set_message(Type type, const std::string& message);

private:
	Type type_;
	time_t time_;
	stdd::optional<int> context_;
	stdd::optional<std::string> location_;
	std::string message_;
};

#endif // !_LOG_ITEM_H_
