#ifndef _JSON_STRING_H_
#define _JSON_STRING_H_

#include "json/value.h"

namespace json {

class String;
using String_ptr = std::shared_ptr<String>;

/*
Represents JSON quoted string
*/
class String final : public Value {
public:
	virtual std::string to_string() const override;
	~String() = default;

	String() = default;
	String(const std::string& value);
	String(std::string&& value);
	static String_ptr create();
	static String_ptr create(const std::string& value);
	static String_ptr create(std::string&& value);

private:
	std::string value_;
};

} // namespace json

#endif // !_JSON_STRING_H_
