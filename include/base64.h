#ifndef _BASE_64_H_
#define _BASE_64_H_

#include <vector>

class Base64 {
public:
	using Byte_buffer_t = std::vector<unsigned char>;
	Byte_buffer_t encode(const Byte_buffer_t& buffer);
	Byte_buffer_t decode(const Byte_buffer_t& buffer);
	Byte_buffer_t decode(const Byte_buffer_t& buffer, size_t buffer_size);
};

#endif // !_BASE_64_H_
