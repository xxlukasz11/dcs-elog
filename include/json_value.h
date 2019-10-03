#ifndef _JSON_VALUE_H_
#define _JSON_VALUE_H_

#include <memory>
#include "json_item.h"

namespace json {

class Json_value : public Json_item {
public:
	virtual std::string to_string() const override = 0;
	~Json_value() = default;
};

using Json_value_ptr = std::shared_ptr<Json_value>;

} // namespace json

#endif // !_JSON_VALUE_H_
