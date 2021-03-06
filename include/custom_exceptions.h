#ifndef _CUSTOM_EXCEPTIONS_H_
#define _CUSTOM_EXCEPTIONS_H_

#include <string>
#include "base_exception.h"
#include "variadic_exception.h"

/*
Error during TCP server initialization
*/
class Init_server_error : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

/*
Error during setting socket property with setsockopt
*/
class Sockopt_error : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

/*
Error when query is not initialized properly
*/
class Query_error : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

/*
Error when receivd message jas unknown id
*/
class Unknown_message : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

/*
Error during parameters loading
*/
class Load_parameters_error : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

/*
Error during writing/reading attachment
*/
class Attachment_error : public Variadic_exception {
	using Variadic_exception::Variadic_exception;
};

/*
Error when received message has incorrect format
*/
class Unknown_message_format : public Base_exception {
public:
	Unknown_message_format(const std::string& message_name);
	Unknown_message_format(const std::string& message_name, const std::string& error_description);
	virtual const std::string& what() const noexcept override;
private:
	std::string message_;
};

/*
Error when readinfg from socket times out
*/
class Timeout_error : public Base_exception {
public:
	Timeout_error();
	virtual const std::string& what() const noexcept override;
private:
	std::string message_;
};

/*
Error when client unexpectedly disconnects
*/
class Client_disconnected_error : public Base_exception {
public:
	Client_disconnected_error();
	virtual const std::string& what() const noexcept override;
private:
	std::string message_;
};

/*
Error while writing data to socket
*/
class Send_error : public Base_exception {
public:
	Send_error();
	virtual const std::string& what() const noexcept override;
private:
	std::string message_;
};

/*
Error while performing operation on the database
*/
class Database_error : public Base_exception {
public:
	Database_error(const std::string& description, int error_code, const char* error_msg);
	Database_error(const std::string& description);
	int get_error_code() const;
	std::string get_error_message();
	virtual const std::string& what() const noexcept override;
private:
	int error_code_;
	std::string message_;
};

#endif