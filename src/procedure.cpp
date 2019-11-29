#include "procedure.h"

Procedure::Procedure(Database& database, const Socket& socket, Attachment_handler_rx& attachment_handler_rx)
	: database_(database), socket_(socket), attachment_handler_rx_(attachment_handler_rx) {
}
