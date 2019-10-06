#ifndef _JSON_JSON_STRING_H_
#define _JSON_JSON_STRING_H_

#include "json/value.h"

namespace json {

class Json_string;
using Json_string_ptr = std::shared_ptr<Json_string>;

class Json_string final : public Value {
public:
	virtual std::string to_string() const override;
	~Json_string() = default;

	Json_string() = default;
	Json_string(const std::string& value);
	Json_string(std::string&& value);
	static Json_string_ptr create(const std::string& value = "");
	static Json_string_ptr create(std::string&& value = "");

private:
	std::string value_;
};

} // namespace json

#endif // !_JSON_STRING_H_
