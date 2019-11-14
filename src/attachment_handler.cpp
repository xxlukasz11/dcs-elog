#include <string>
#include <fstream>
#include "attachment_handler.h"
#include "logger.h"

constexpr int RX_BUFFER_SIZE = 100000;

Attachment_handler::Attachment_handler(Socket socket) : socket_(socket) {
}

void Attachment_handler::handle_attachments(const Attachment_info_array& attachments_info) {
	for (const auto& att_info : attachments_info) {
		receive_and_save_attachment(att_info);
	}
}

void Attachment_handler::receive_and_save_attachment(const Attachment_info& attachment_info) {
	std::string file_name = attachment_info.get_name();
	std::ofstream file(file_name, std::ios::binary);
	std::vector<uint8_t> buffer(RX_BUFFER_SIZE);

	int file_size = socket_.receive<int>();
	int bytes_sent = 0;
	while (bytes_sent < file_size) {
		int bytes_to_save = std::min(RX_BUFFER_SIZE, file_size - bytes_sent);
		socket_.fill_buffer(buffer, bytes_to_save);
		file.write(reinterpret_cast<const char*>(buffer.data()), bytes_to_save);
		bytes_sent += bytes_to_save;
	}
	file.close();
}
