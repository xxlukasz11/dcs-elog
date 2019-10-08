#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include <utility>
#include <string>
#include "database.h"
#include "socket.h"
#include "website_response.h"
#include "json_stringifier.h"

class Procedure {
public:
	Procedure(Database& database, const Socket& socket);
	virtual ~Procedure() = default;
	virtual void start() = 0;
	virtual std::string name() = 0;
	
protected:
	template<typename T>
	std::enable_if_t<std::is_base_of<Response, std::remove_reference_t<T>>::value, void>
	send_response(T&& response);

	Database& database_;
	Socket socket_;
};

template<typename T>
std::enable_if_t<std::is_base_of<Response, std::remove_reference_t<T>>::value, void>
inline Procedure::send_response(T&& response) {
	socket_.send_string(
		Json_stringifier::stringify(
			std::forward<T>(response)));
}

#endif // !_PROCEDURE_H_
