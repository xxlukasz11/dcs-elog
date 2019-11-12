#include <string>
#include <fstream>
#include "attachment_handler.h"
#include "logger.h"

constexpr uint32_t RX_BUFFER_SIZE = 10000;

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

	uint32_t file_size = socket_.receive<uint32_t>();
	uint32_t bytes_to_receive = file_size;
	while (bytes_to_receive > 0) {
		uint32_t bytes_to_save = std::min(RX_BUFFER_SIZE, bytes_to_receive);
		socket_.fill_buffer(buffer, bytes_to_save);
		file.write(reinterpret_cast<const char*>(buffer.data()), bytes_to_save);
	}
	file.close();
}
