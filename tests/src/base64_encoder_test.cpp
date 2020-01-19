#include "gtest/gtest.h"
#include "base64_encoder.h"

namespace{

using Buffer_t = Base64_encoder::Byte_buffer_t;

Buffer_t create_string_buffer(const char c1, const char c2, const char c3, const char c4) {
	Buffer_t buffer(4);
	buffer[0] = c1;
	buffer[1] = c2;
	buffer[2] = c3;
	buffer[3] = c4;
	return buffer;
}

Buffer_t concatenate_buffers(const Buffer_t& b1, const Buffer_t& b2) {
	Buffer_t buffer;
	buffer.reserve(b1.size() + b2.size());
	buffer.insert(buffer.end(), b1.begin(), b1.end());
	buffer.insert(buffer.end(), b2.begin(), b2.end());
	return buffer;
}

} // namespace

static const std::string chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
static const char PADDING = '=';

static const Buffer_t SIMPLE_BYTE_BUFFER = Buffer_t{ 0x04, 0x10, 0x41 }; // 1 1 1 1
static const Buffer_t SIMPLE_STRING_BUFFER = create_string_buffer(chars[1], chars[1], chars[1], chars[1]);

static const Buffer_t SMALLER_BYTE_BUFFER = Buffer_t{ 0x04, 0x10}; // 1 1 0 x
static const Buffer_t SMALLER_STRING_BUFFER = create_string_buffer(chars[1], chars[1], chars[0], PADDING);

static const Buffer_t SMALLEST_BYTE_BUFFER = Buffer_t{ 0x04 }; // 1 0 x x
static const Buffer_t SMALLEST_STRING_BUFFER = create_string_buffer(chars[1], chars[0], PADDING, PADDING);

static const Buffer_t DOUBLE_STRING_BUFFER = concatenate_buffers(SIMPLE_STRING_BUFFER, SIMPLE_STRING_BUFFER);
static const Buffer_t EMPTY_BUFFER;

static const Base64_encoder encoder;

// encoding
TEST(base64_encoder, encode_simple) {
	auto result = encoder.encode(SIMPLE_BYTE_BUFFER);
	ASSERT_EQ(SIMPLE_STRING_BUFFER, result);
}

TEST(base64_encoder, encode_single_padding) {
	auto result = encoder.encode(SMALLER_BYTE_BUFFER);
	ASSERT_EQ(SMALLER_STRING_BUFFER, result);
}

TEST(base64_encoder, encode_double_padding) {
	auto result = encoder.encode(SMALLEST_BYTE_BUFFER);
	ASSERT_EQ(SMALLEST_STRING_BUFFER, result);
}

TEST(base64_encoder, encode_empty_buffer_when_empty) {
	auto result = encoder.encode(EMPTY_BUFFER);
	ASSERT_EQ(EMPTY_BUFFER, result);
}

// decoding
TEST(base64_encoder, decode_simple) {
	auto result = encoder.decode(SIMPLE_STRING_BUFFER);
	ASSERT_EQ(SIMPLE_BYTE_BUFFER, result);
}

TEST(base64_encoder, decode_single_padding) {
	auto result = encoder.decode(SMALLER_STRING_BUFFER);
	ASSERT_EQ(SMALLER_BYTE_BUFFER, result);
}

TEST(base64_encoder, decode_double_padding) {
	auto result = encoder.decode(SMALLEST_STRING_BUFFER);
	ASSERT_EQ(SMALLEST_BYTE_BUFFER, result);
}

TEST(base64_encoder, decode_half_of_bytes) {
	auto result = encoder.decode(DOUBLE_STRING_BUFFER, SIMPLE_STRING_BUFFER.size());
	ASSERT_EQ(SIMPLE_BYTE_BUFFER, result);
}

TEST(base64_encoder, decode_empty_buffer_when_not_divided_by_4) {
	auto result = encoder.decode(SIMPLE_STRING_BUFFER, 3);
	ASSERT_EQ(EMPTY_BUFFER, result);
}

TEST(base64_encoder, decode_empty_buffer_when_empty) {
	auto result = encoder.decode(EMPTY_BUFFER);
	ASSERT_EQ(EMPTY_BUFFER, result);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
