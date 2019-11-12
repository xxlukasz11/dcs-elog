#include "attachment_handler.h"
#include "logger.h"

Attachment_handler::Attachment_handler(Socket socket) : socket_(socket) {
}

void Attachment_handler::handle_attachments(const Attachment_info_array& attachments_info) {
	attachments_info_ = attachments_info;
	Logger::create().info("Managing attachments");

	// TODO
}
