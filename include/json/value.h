#ifndef _JSON_VALUE_H_
#define _JSON_VALUE_H_

#include <memory>
#include "json/item.h"

namespace json {

class Value : public Item {
public:
	virtual std::string to_string() const override = 0;
	~Value() = default;
};

using Value_ptr = std::shared_ptr<Value>;

} // namespace json

#endif // !_JSON_VALUE_H_
