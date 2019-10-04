#ifndef _JSON_FIELD_H_
#define _JSON_FIELD_H_

#include "json/item.h"
#include "json/value.h"
#include <memory>
#include <string>

namespace json {

class Field final : public Item {
public:
	virtual std::string to_string() const override;
	~Field() = default;

	Field(const std::string& label, const Value_ptr& value);
private:
	std::string label_;
	Value_ptr value_;
};

using Field_ptr = std::shared_ptr<Field>;

} // namespace json

#endif // !_JSON_FIELD_H_
