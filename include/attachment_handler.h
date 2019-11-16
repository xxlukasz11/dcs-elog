#ifndef _ATTACHMENT_HANDLER_H_
#define _ATTACHMENT_HANDLER_H_

#include "attachment_info.h"
#include "socket.h"
#include <atomic>

class Attachment_handler {
public:
	Attachment_handler(Socket socket);

	// Rx

	void handle_attachments(const Attachment_info_array& attachments_info_);
	void receive_and_save_attachment(const Attachment_info& attachment_info);


private:
	std::ofstream create_unique_file(const std::string& file_name);
	std::string generate_file_name_discriminator();

	static std::atomic<unsigned long> attachment_control_index_;
	Socket socket_;
};

#endif // !_ATTACHMENT_HANDLER_H_
