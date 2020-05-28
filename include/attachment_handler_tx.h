#ifndef _ATTACHMENT_HANDLER_TX_
#define _ATTACHMENT_HANDLER_TX_

#include "result_set.h"
#include "socket.h"
#include <string>

/*
Reads attachments and send them
*/
class Attachment_handler_tx {
public:
	Attachment_handler_tx(Socket socket);
	void inject_payload_and_send();
	void set_events(Result_set&& events);
	void set_attachments(Result_set&& attachments);
	void set_on_success_message(const std::string message);
	void set_on_failure_message(const std::string message);
private:
	std::string create_events_string();
	std::string create_response_string();
	void assert_valid_attachments_size(int size);
	void send_plain(const std::string& string);
	void send_with_attachment_payload(std::string&& string);
	void send_attachment(const std::string& file_name);
	void move_attachments_file_names(Result_set&& attachments);

	Socket socket_;
	Result_set events_;
	Result_set attachments_;
	std::vector<std::string> sequenced_attachments_;
	std::string on_success_message_;
	std::string on_failure_message_;
};

#endif // !_ATTACHMENT_HANDLER_TX_
