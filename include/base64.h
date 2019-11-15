#ifndef _BASE_64_H_
#define _BASE_64_H_

#include <vector>

class Base64 {
public:
	using String_buffer = std::vector<char>;
	String_buffer encode(const String_buffer& buffer);
	String_buffer decode(const String_buffer& buffer);
	String_buffer decode(const String_buffer& buffer, size_t buffer_size);
};

#endif // !_BASE_64_H_
