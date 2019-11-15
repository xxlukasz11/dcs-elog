#include <string>
#include <fstream>
#include "attachment_handler.h"
#include "logger.h"
#include "base64.h"

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
	std::vector<char> buffer(RX_BUFFER_SIZE);

	int file_size = socket_.receive<int>();
	Logger::create().info("Receiving " + file_name);

	Base64 base64;
	int bytes_received = 0;
	int binary_file_size = 0;
	while (bytes_received < file_size) {
		int current_buffer_size = std::min(RX_BUFFER_SIZE, file_size - bytes_received);
		socket_.fill_buffer(buffer, current_buffer_size);
		bytes_received += current_buffer_size;
		auto decoded = base64.decode(buffer, current_buffer_size);
		binary_file_size += decoded.size();
		file.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
	}
	file.close();
	Logger::create().info("File " + file_name + " successfully received; Size: " + std::to_string(binary_file_size) + " bytes");
}
