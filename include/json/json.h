#ifndef _JSON_H_
#define _JSON_H_

#include "json/array.h"
#include "json/object.h"
#include "json/string.h"
#include "json/number.h"
#include "json/json_string.h"

namespace json {

class Json {
public:
	template<typename T>
	static Number_ptr<T> number(T value);
	static Array_ptr array();
	static Object_ptr object();
	static String_ptr string(const std::string& value);
	static String_ptr string(std::string&& value);
	static Json_string_ptr json_string(const std::string& value);
	static Json_string_ptr json_string(std::string&& value);
};

template<typename T>
inline Number_ptr<T> Json::number(T value) {
	return Number<T>::create(value);
}

}

#endif // !_JSO_H_
