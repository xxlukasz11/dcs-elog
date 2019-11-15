#include <string>
#include "base64.h"

static const std::string chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

constexpr char FIRST_TWO = 0xc0;
constexpr char LAST_TWO = 0x03;
constexpr char FIRST_FOUR = 0xf0;
constexpr char LAST_FOUR = 0x0f;
constexpr char FIRST_SIX = 0xfc;
constexpr char LAST_SIX = 0x3f;

Base64::String_buffer Base64::encode(const String_buffer& buffer) {
	auto n = buffer.size();
	if (n == 0) {
		return {};
	}

	auto iterations = n / 3;
	auto padding = n % 3;

	int encoded_length = 4 * (iterations + (padding ? 1 : 0));
	std::vector<char> encoded_buffer;
	encoded_buffer.reserve(encoded_length);

	for (int i = 0; i < iterations; ++i) {
		int index = i * 3;
		encoded_buffer.push_back(chars[(buffer[index] & FIRST_SIX) >> 2]);
		encoded_buffer.push_back(chars[((buffer[index] & LAST_TWO) << 4) | ((buffer[index + 1] & FIRST_FOUR) >> 4)]);
		encoded_buffer.push_back(chars[((buffer[index + 1] & LAST_FOUR) << 2) | ((buffer[index + 2] & FIRST_TWO) >> 6)]);
		encoded_buffer.push_back(chars[buffer[index + 2] & LAST_SIX]);
	}

	if (padding != 0) {
		int index = n - padding;
		encoded_buffer.push_back(chars[(buffer[index] & FIRST_SIX) >> 2]);

		if (padding == 1) {
			encoded_buffer.push_back(chars[(buffer[index] & LAST_TWO) << 4]);
			encoded_buffer.push_back('=');
			encoded_buffer.push_back('=');
		}
		else if (padding == 2) {
			encoded_buffer.push_back(chars[((buffer[index] & LAST_TWO) << 4) | ((buffer[index + 1] & FIRST_FOUR) >> 4)]);
			encoded_buffer.push_back(chars[(buffer[index + 1] & LAST_FOUR) << 2]);
			encoded_buffer.push_back('=');
		}
	}

	return encoded_buffer;
}

Base64::String_buffer Base64::decode(const String_buffer& buffer, size_t buffer_size) {
	if (buffer_size == 0) {
		return {};
	}

	auto iterations = buffer_size / 4;
	int decoded_length = 3 * iterations;
	std::vector<char> decoded_buffer;
	decoded_buffer.reserve(decoded_length);

	for (int i = 0; i < iterations - 1; ++i) {
		int index = i * 4;
		char l1 = chars.find(buffer[index]);
		char l2 = chars.find(buffer[index + 1]);
		char l3 = chars.find(buffer[index + 2]);
		char l4 = chars.find(buffer[index + 3]);
		decoded_buffer.push_back((l1 << 2) | (l2 >> 4));
		decoded_buffer.push_back((l2 << 4) | (l3 >> 2));
		decoded_buffer.push_back((l3 << 6) | l4);
	}

	char l1 = chars.find(buffer[buffer_size - 4]);
	char l2 = chars.find(buffer[buffer_size - 3]);
	decoded_buffer.push_back((l1 << 2) | (l2 >> 4));

	char l3 = chars.find(buffer[buffer_size - 2]);
	if ((int)l3 != std::string::npos) {
		decoded_buffer.push_back((l2 << 4) | (l3 >> 2));
		char l4 = chars.find(buffer[buffer_size - 1]);
		if ((int)l4 != std::string::npos) {
			decoded_buffer.push_back((l3 << 6) | l4);
		}
	}

	return decoded_buffer;
}

Base64::String_buffer Base64::decode(const String_buffer& buffer) {
	return decode(buffer, buffer.size());
}
