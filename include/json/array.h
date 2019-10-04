#ifndef _JSON_ARRAY_H_
#define _JSON_ARRAY_H_

#include "json/value.h"
#include "json/field.h"

namespace json {

class Array;
using Array_ptr = std::shared_ptr<Array>;

class Array final : public Value {
public:
	virtual std::string to_string() const override;
	~Array() = default;

	static Array_ptr create();
	void push(const Value_ptr& value);
private:
	std::string values_to_string() const;

	std::vector<Value_ptr> values_;
};

} // namespace json

#endif // !_JSON_ARRAY_H_