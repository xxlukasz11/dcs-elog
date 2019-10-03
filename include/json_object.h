#ifndef _JSON_OBJECT_H_
#define _JSON_OBJECT_H_

#include "json_value.h"
#include "json_field.h"

namespace json {

class Json_object;
using Json_object_ptr = std::shared_ptr<Json_object>;

class Json_object final : public Json_value {
public:
	virtual std::string to_string() const override;
	~Json_object() = default;

	void add_field(const std::string& name, const Json_value_ptr& value);
	static Json_object_ptr create();
private:
	std::string fields_to_string() const;

	Array<Json_field> fields_;
};

} // namespace json

#endif // !_JSON_OBJECT_H_
