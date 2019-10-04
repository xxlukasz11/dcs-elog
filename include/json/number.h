#ifndef _JSON_NUMBER_H_
#define _JSON_NUMBER_H_

#include "json/value.h"
#include <type_traits>

namespace json {

template<typename T>
using arithmetic_t = typename std::enable_if_t<std::is_arithmetic<T>::value, int>;

template<typename T, arithmetic_t<T> = 0>
class Number;

template<typename T, arithmetic_t<T> = 0>
using Number_ptr = std::shared_ptr<Number<T>>;

template<typename T, arithmetic_t<T>>
class Number final : public Value {
public:
	std::string to_string() const override {
		return std::to_string(value_);
	}
	~Number() = default;

	Number(T value) : value_(value) {
	}

	static Number_ptr<T> create(T value) {
		return std::make_shared<Number>(value);
	}

private:
	T value_;
};

} // namespace json

#endif // !_JSON_NUMBER_H_
