#ifndef _JSON_STRINGIFIER_H_
#define _JSON_STRINGIFIER_H_

#include <string>
#include "result_set.h"
#include "website_response.h"
#include "server_response.h"
#include "json/json.h"

class Json_stringifier {
public:
	static json::Array_ptr to_json_array(const Result_set& result_set);
	static json::Array_ptr to_json_array(Result_set&& result_set);
	static std::string stringify(const Result_set& result_set);
	static std::string stringify(Result_set&& result_set);
	static std::string stringify(const Website_response& response);
	static std::string stringify(Website_response&& response);
	static std::string stringify(const Server_response& response);
};

#endif // !_JSON_STRINGIFIER_H_
