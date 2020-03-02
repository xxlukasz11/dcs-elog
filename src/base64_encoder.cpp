#include <string>
#include "base64_encoder.h"
#include "logger.h"

static const std::string chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";
static const char PADDING = '=';

constexpr unsigned char FIRST_TWO = 0xc0;
constexpr unsigned char LAST_TWO = 0x03;
constexpr unsigned char FIRST_FOUR = 0xf0;
constexpr unsigned char LAST_FOUR = 0x0f;
constexpr unsigned char FIRST_SIX = 0xfc;
constexpr unsigned char LAST_SIX = 0x3f;

Base64_encoder::Byte_buffer_t Base64_encoder::encode(const Byte_buffer_t& buffer) const {
	const auto bfSize = buffer.size();
	if (bfSize == 0) {
		return {};
	}

	const auto iterations = bfSize / 3;
	const auto padding = bfSize % 3;

	const int encoded_length = 4 * (iterations + (padding ? 1 : 0));
	Byte_buffer_t encoded_buffer;
	encoded_buffer.reserve(encoded_length);

	for (int i = 0; i < iterations; ++i) {
		const auto index = i * 3;
		encoded_buffer.push_back(chars[(buffer[index] & FIRST_SIX) >> 2]);
		encoded_buffer.push_back(chars[((buffer[index] & LAST_TWO) << 4) | ((buffer[index + 1] & FIRST_FOUR) >> 4)]);
		encoded_buffer.push_back(chars[((buffer[index + 1] & LAST_FOUR) << 2) | ((buffer[index + 2] & FIRST_TWO) >> 6)]);
		encoded_buffer.push_back(chars[buffer[index + 2] & LAST_SIX]);
	}

	if (padding != 0) {
		const auto index = bfSize - padding;
		encoded_buffer.push_back(chars[(buffer[index] & FIRST_SIX) >> 2]);

		if (padding == 1) {
			encoded_buffer.push_back(chars[(buffer[index] & LAST_TWO) << 4]);
			encoded_buffer.push_back(PADDING);
			encoded_buffer.push_back(PADDING);
		}
		else if (padding == 2) {
			encoded_buffer.push_back(chars[((buffer[index] & LAST_TWO) << 4) | ((buffer[index + 1] & FIRST_FOUR) >> 4)]);
			encoded_buffer.push_back(chars[(buffer[index + 1] & LAST_FOUR) << 2]);
			encoded_buffer.push_back(PADDING);
		}
	}

	return encoded_buffer;
}

Base64_encoder::Byte_buffer_t Base64_encoder::decode(const Byte_buffer_t& buffer, size_t buffer_size) const {
	if (buffer_size == 0 || buffer_size % 4 != 0) {
		return {};
	}

	const auto iterations = buffer_size / 4;
	const int decoded_length = 3 * iterations;
	Byte_buffer_t decoded_buffer;
	decoded_buffer.reserve(decoded_length);

	for (int i = 0; i < iterations - 1; ++i) {
		int index = i * 4;
		const char l1 = chars.find(buffer[index]);
		const char l2 = chars.find(buffer[index + 1]);
		const char l3 = chars.find(buffer[index + 2]);
		const char l4 = chars.find(buffer[index + 3]);
		decoded_buffer.push_back((l1 << 2) | (l2 >> 4));
		decoded_buffer.push_back((l2 << 4) | (l3 >> 2));
		decoded_buffer.push_back((l3 << 6) | l4);
	}

	const char l1 = chars.find(buffer[buffer_size - 4]);
	const char l2 = chars.find(buffer[buffer_size - 3]);
	decoded_buffer.push_back((l1 << 2) | (l2 >> 4));

	const char l3 = chars.find(buffer[buffer_size - 2]);
	if ((int)l3 != std::string::npos) {
		decoded_buffer.push_back((l2 << 4) | (l3 >> 2));
		const char l4 = chars.find(buffer[buffer_size - 1]);
		if ((int)l4 != std::string::npos) {
			decoded_buffer.push_back((l3 << 6) | l4);
		}
	}

	return decoded_buffer;
}

Base64_encoder::Byte_buffer_t Base64_encoder::decode(const Byte_buffer_t& buffer) const {
	return decode(buffer, buffer.size());
}
