#include "payload.h"

void Payload::append(Byte_buffer_t&& buffer) {
	byte_buffer_.insert(byte_buffer_.end(),
		std::make_move_iterator(buffer.begin()),
		std::make_move_iterator(buffer.end()));
	buffer.erase(buffer.begin(), buffer.end());
}

Byte_buffer_t& Payload::get_byte_buffer() {
	return byte_buffer_;
}
