#ifndef _JSON_OBJECT_H_
#define _JSON_OBJECT_H_

#include "json/value.h"
#include "json/field.h"

namespace json {

class Object;
using Object_ptr = std::shared_ptr<Object>;


/*
Represents JSON object
*/
class Object final : public Value {
public:
	virtual std::string to_string() const override;
	~Object() = default;

	void add_field(const std::string& name, const Value_ptr& value);
	static Object_ptr create();
private:
	std::string fields_to_string() const;

	std::vector<Field> fields_;
};

} // namespace json

#endif // !_JSON_OBJECT_H_
