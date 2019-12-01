#include "procedure.h"
#include "logger.h"

Procedure::Procedure(Database& database, const Socket& socket,
	Attachment_handler_rx& attachment_handler_rx,
	Attachment_handler_tx& attachment_handler_tx)
	: database_(database), socket_(socket),
	attachment_handler_rx_(attachment_handler_rx),
	attachment_handler_tx_(attachment_handler_tx) {
}

void Procedure::log_response_message(const Website_response& response) {
	Logger::create().context(socket_).level(Log_level::INFO).info(
		"Response message: " + response.get_message());
}
