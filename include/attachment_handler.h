#ifndef _ATTACHMENT_HANDLER_H_
#define _ATTACHMENT_HANDLER_H_

#include "attachment_info.h"
#include "socket.h"

class Attachment_handler {
public:
	Attachment_handler(Socket socket);
	void handle_attachments(const Attachment_info_array& attachments_info_);

private:
	Socket socket_;
	Attachment_info_array attachments_info_;
};

#endif // !_ATTACHMENT_HANDLER_H_
