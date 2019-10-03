#ifndef _JSON_FIELD_H_
#define _JSON_FIELD_H_

#include "json_item.h"
#include "json_value.h"
#include <memory>

namespace json {

class Json_field final : public Json_item {
public:
	virtual std::string to_string() const override;
	~Json_field() = default;

	Json_field(const std::string& label, const Json_value_ptr& value);
private:
	std::string label_;
	Json_value_ptr value_;
};

} // namespace json

#endif // !_JSON_FIELD_H_
