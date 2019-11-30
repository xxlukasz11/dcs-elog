#include "procedure.h"

Procedure::Procedure(Database& database, const Socket& socket,
	Attachment_handler_rx& attachment_handler_rx,
	Attachment_handler_tx& attachment_handler_tx)
	: database_(database), socket_(socket),
	attachment_handler_rx_(attachment_handler_rx),
	attachment_handler_tx_(attachment_handler_tx) {
}
