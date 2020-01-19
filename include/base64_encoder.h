#ifndef _BASE64_ENCODER_H_
#define _BASE64_ENCODER_H_

#include <vector>

class Base64_encoder {
public:
	using Byte_buffer_t = std::vector<unsigned char>;

	// Encode to base64 string buffer
	Byte_buffer_t encode(const Byte_buffer_t& buffer) const;

	// Decode from base64 string buffer
	Byte_buffer_t decode(const Byte_buffer_t& buffer) const;

	// Decode given amount of bytes from base64 string buffer
	Byte_buffer_t decode(const Byte_buffer_t& buffer, size_t bytes_to_decode) const;
};

#endif // !_BASE64_ENCODER_H_
