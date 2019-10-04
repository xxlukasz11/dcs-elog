#ifndef _JSON_H_
#define _JSON_H_

#include "json/array.h"
#include "json/object.h"
#include "json/string.h"
#include "json/number.h"

namespace json {

class Json {
public:
	template<typename T>
	static Number_ptr<T> number(T value);
	static String_ptr string(const std::string& value);
	static Array_ptr array();
	static Object_ptr object();
};

template<typename T>
inline Number_ptr<T> Json::number(T value) {
	return Number<T>::create(value);
}

}

#endif // !_JSO_H_
