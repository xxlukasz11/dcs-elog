#ifndef _ATTACHMENT_HANDLER_RX_H_
#define _ATTACHMENT_HANDLER_RX_H_

#include "attachment_info.h"
#include "attachment_database_info.h"
#include "socket.h"
#include <atomic>

class Attachment_handler_rx {
public:
	Attachment_handler_rx(Socket socket);
	~Attachment_handler_rx();
	void handle_attachments(const Attachment_info_array& attachments_info_);
	void receive_and_save_attachment(const Attachment_info& attachment_info);
	void commit();
	const Attachment_database_info_array& get_attachment_array() const;

private:
	void delete_attachments_from_disc();
	std::ofstream create_unique_file(const Attachment_info& attachment_info);
	std::string generate_file_name_discriminator();
	std::string create_attachment_path(const std::string& file_name);
	std::string create_unique_file_name(const std::string& file_name);
	int receive_attachment_size();
	void log_incoming_attachment_info(const Attachment_info& attachment_info, int file_size);

	static std::atomic<unsigned long> attachment_control_index_;
	Attachment_database_info_array attachment_array_;
	Socket socket_;
	bool commited_{ false };
};

#endif // !_ATTACHMENT_HANDLER_RX_H_
