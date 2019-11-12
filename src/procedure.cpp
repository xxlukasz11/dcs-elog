#include "procedure.h"

Procedure::Procedure(Database& database, const Socket& socket, Attachment_handler& attachment_handler)
	: database_(database), socket_(socket), attachment_handler_(attachment_handler) {
}
