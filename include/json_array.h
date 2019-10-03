#ifndef _JSON_ARRAY_H_
#define _JSON_ARRAY_H_

#include "json_value.h"
#include "json_field.h"

namespace json {

class Json_array;
using Json_array_ptr = std::shared_ptr<Json_array>;

class Json_array final : public Json_value {
public:
	virtual std::string to_string() const override;
	~Json_array() = default;

	static Json_array_ptr create();
	void push(const Json_value_ptr& value);
private:
	std::string values_to_string() const;

	Array<Json_value_ptr> values_;
};

} // namespace json

#endif // !_JSON_ARRAY_H_