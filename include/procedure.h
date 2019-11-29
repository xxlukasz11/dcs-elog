#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include <utility>
#include <string>
#include "database.h"
#include "socket.h"
#include "website_response.h"
#include "json_stringifier.h"
#include "attachment_handler_rx.h"
#include "message.h"

class Procedure {
public:
	Procedure(Database& database, const Socket& socket, Attachment_handler_rx& attachment_handler_rx);
	virtual ~Procedure() = default;
	virtual void start() = 0;
	virtual std::string name() = 0;
	virtual void set_message(const std::shared_ptr<Message>& message) = 0;
	
protected:
	template<typename T>
	std::enable_if_t<std::is_base_of<Response, std::remove_reference_t<T>>::value, void>
	send_response(T&& response);

	Database& database_;
	Socket socket_;
	Attachment_handler_rx& attachment_handler_rx_;
};

template<typename T>
std::enable_if_t<std::is_base_of<Response, std::remove_reference_t<T>>::value, void>
inline Procedure::send_response(T&& response) {
	socket_.send_string(
		Json_stringifier::stringify(
			std::forward<T>(response)));
}

#endif // !_PROCEDURE_H_
