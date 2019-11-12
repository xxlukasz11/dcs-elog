#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

#include <vector>
#include <cstdint>

using Byte_t = uint8_t;
using Byte_buffer_t = std::vector<Byte_t>;

class Payload {
public:
	void append(Byte_buffer_t&& buffer);
	Byte_buffer_t& get_byte_buffer();

private:
	Byte_buffer_t byte_buffer_;

};

#endif // !_PAYLOAD_H_
