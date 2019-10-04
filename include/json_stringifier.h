#ifndef _JSON_STRINGIFIER_H_
#define _JSON_STRINGIFIER_H_

#include <string>
#include "result_set.h"

class Json_stringifier {
public:
	static std::string stringify(const Result_set& result_set);
	static std::string stringify(Result_set&& result_set);
};

#endif // !_JSON_STRINGIFIER_H_
