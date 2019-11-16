#include <string>
#include <fstream>
#include "attachment_handler.h"
#include "logger.h"
#include "base64.h"
#include "file_name_parser.h"
#include "custom_exceptions.h"
#include "administrator.h"
#include "utils.h"

constexpr int RX_BUFFER_SIZE = 100000;
static const std::string FILE_NAME_FORMAT = "%Y_%m_%d_%H_%M_%S";
static const std::string UNDERSCORE = "_";

std::atomic<unsigned long> Attachment_handler::attachment_control_index_{ 0 };

Attachment_handler::Attachment_handler(Socket socket) : socket_(socket) {
}

void Attachment_handler::handle_attachments(const Attachment_info_array& attachments_info) {
	for (const auto& att_info : attachments_info) {
		receive_and_save_attachment(att_info);
	}
}

void Attachment_handler::receive_and_save_attachment(const Attachment_info& attachment_info) {
	std::string file_name = attachment_info.get_name();
	std::ofstream file = create_unique_file(file_name);
	std::vector<char> buffer(RX_BUFFER_SIZE);

	int file_size = socket_.receive<int>();
	Logger::create().level(Log_level::ALL).info("Receiving " + file_name);

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

std::ofstream Attachment_handler::create_unique_file(const std::string& file_name) {
	std::string unique_file_name = create_unique_file_name(file_name);
	std::string path = create_attachment_path(unique_file_name);
	Logger::create().level(Log_level::ALL).info("Saving " + file_name + " in " + path);
	return std::ofstream(path, std::ios::binary);
}

std::string Attachment_handler::generate_file_name_discriminator() {
	return std::to_string(attachment_control_index_++);
}

std::string Attachment_handler::create_attachment_path(const std::string& file_name) {
	return Administrator::instance().params().get_attachment_storage_path() + file_name;
}

std::string Attachment_handler::create_unique_file_name(const std::string& file_name) {
	File_name_parser parser;
	if (!parser.parse(file_name)) {
		throw Attachment_error("Invalid file name: ", file_name);
	}
	time_t now = std::time(0);
	return utils::create_date_time_string(now, FILE_NAME_FORMAT)
		+ UNDERSCORE
		+ generate_file_name_discriminator()
		+ parser.get_extension();
}
