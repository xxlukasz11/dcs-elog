#include <exception>
#include "file_stream.h"

File_stream::File_stream(std::string file_name) : file_name_(file_name), stream_(std::make_shared<std::ofstream>()) {
	if (!clear_file()) {
		throw std::runtime_error("Could not access log file: " + file_name_);
	}
}

void File_stream::open() {
	if (stream_->is_open()) {
		close();
	}
	stream_->open(file_name_, std::fstream::app);
}

void File_stream::open_if_closed() {
	if (!stream_->is_open()) {
		stream_->open(file_name_, std::fstream::app);
	}
}

void File_stream::close() {
	stream_->close();
}

void File_stream::flush() {
	stream_->flush();
}

bool File_stream::clear_file() {
	stream_->open(file_name_, std::fstream::out);
	if (stream_->fail()) {
		return false;
	}
	close();
	return true;
}
