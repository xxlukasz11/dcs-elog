#ifndef _JSON_H_
#define _JSON_H_

#include "json_array.h"
#include "json_object.h"
#include "json_string.h"
#include "json_number.h"

namespace json {

class Json {
public:
	template<typename T>
	static Json_number_ptr<T> number(T value);
	static Json_string_ptr string(const std::string& value);
	static Json_array_ptr array();
	static Json_object_ptr object();
};

template<typename T>
inline Json_number_ptr<T> Json::number(T value) {
	return Json_number<T>::create(value);
}

}

#endif // !_JSO_H_
