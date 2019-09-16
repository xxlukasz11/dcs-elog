#include "procedure.h"

Procedure::Procedure(Database& database, const Socket& socket)
	: database_(database), socket_(socket) {
}
