#ifndef _JSON_H_
#define _JSON_H_

#include <vector>
#include <string>

#include "result_set.h"

namespace json{
	std::string stringify(const Result_set& result_set);
	std::string stringify(Result_set&& result_set);
}

#endif