#ifndef _BASE64_ENCODER_H_
#define _BASE64_ENCODER_H_

#include <vector>

class Base64_encoder {
public:
	using Byte_buffer_t = std::vector<unsigned char>;
	Byte_buffer_t encode(const Byte_buffer_t& buffer);
	Byte_buffer_t decode(const Byte_buffer_t& buffer);
	Byte_buffer_t decode(const Byte_buffer_t& buffer, size_t buffer_size);
};

#endif // !_BASE64_ENCODER_H_
