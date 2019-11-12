#ifndef _ATTACHMENT_HANDLER_H_
#define _ATTACHMENT_HANDLER_H_

#include "attachment_info.h"
#include "socket.h"

class Attachment_handler {
public:
	Attachment_handler(Socket socket);

	// Rx

	void handle_attachments(const Attachment_info_array& attachments_info_);
	void receive_and_save_attachment(const Attachment_info& attachment_info);


private:
	Socket socket_;
};

#endif // !_ATTACHMENT_HANDLER_H_
