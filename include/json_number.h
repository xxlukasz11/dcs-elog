#ifndef _JSON_NUMBER_H_
#define _JSON_NUMBER_H_

#include "json_value.h"
#include <type_traits>

namespace json {

template<typename T>
using arithmetic_t = typename std::enable_if_t<std::is_arithmetic<T>::value, int>;

template<typename T, arithmetic_t<T> = 0>
class Json_number;

template<typename T, arithmetic_t<T> = 0>
using Json_number_ptr = std::shared_ptr<Json_number<T>>;

template<typename T, arithmetic_t<T>>
class Json_number final : public Json_value {
public:
	std::string to_string() const override {
		return std::to_string(value_);
	}
	~Json_number() = default;

	Json_number(T value) : value_(value) {
	}

	static Json_number_ptr<T> create(T value) {
		return std::make_shared<Json_number>(value);
	}

private:
	T value_;
};

} // namespace json

#endif // !_JSON_NUMBER_H_
