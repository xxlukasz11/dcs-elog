#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include <string>
#include "database.h"
#include "socket.h"

class Procedure {
public:
	Procedure(Database& database, const Socket& socket);
	virtual ~Procedure() = default;
	virtual void start() = 0;
	virtual std::string name() = 0;
	
protected:
	Database& database_;
	Socket socket_;
};

#endif // !_PROCEDURE_H_
