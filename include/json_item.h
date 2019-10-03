#ifndef _JSON_ITEM_H_
#define _JSON_ITEM_H_

#include <string>
#include <vector>

namespace json {

class Json_item {
public:
	virtual std::string to_string() const = 0;
	~Json_item() = default;

protected:
	template<typename T>
	using Array = std::vector<T>;

	static const std::string DOUBLE_QUOTES;
	static const std::string COLON;
	static const std::string COMMA;
	static const std::string CURLY_BRACE_OPEN;
	static const std::string CURLY_BRACE_CLOSE;
	static const std::string SQUARE_BRACE_OPEN;
	static const std::string SQUARE_BRACE_CLOSE;
};

} // namespace json

#endif // !_JSON_ITEM_H_
