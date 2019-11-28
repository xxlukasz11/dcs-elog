#include "gtest/gtest.h"
#include "utils.h"
#include "log_level.h"

static const std::string SPACE = " ";
static const std::string COMMA_SPACE = ", ";
static const std::string EMPTY_STRING;
static const std::string WORD_1 = "word_1";
static const std::string WORD_2 = "word_2_";
static const std::string WORD_3 = "word_3__";
static const utils::String_array STRING_ARRAY_123{ WORD_1, WORD_2, WORD_3 };
static const utils::String_array STRING_ARRAY_13{ WORD_1, WORD_3 };
static const utils::String_array STRING_ARRAY_EMPTY;

TEST(concatenate_string_array, simple) {
	auto concatenated = utils::concatenate_string_array(STRING_ARRAY_123);
	auto expected = WORD_1 + COMMA_SPACE + WORD_2 + COMMA_SPACE + WORD_3;
	EXPECT_EQ(expected, concatenated);
}

TEST(exclude_from_array, leave_middle_element) {
	auto processed = utils::exclude_from_array(STRING_ARRAY_123, STRING_ARRAY_13);
	EXPECT_EQ(1, processed.size());
	EXPECT_EQ(WORD_2, processed[0]);
}

TEST(exclude_from_array, leave_all) {
	auto processed = utils::exclude_from_array(STRING_ARRAY_123, STRING_ARRAY_EMPTY);
	EXPECT_EQ(STRING_ARRAY_123.size(), processed.size());
}

TEST(create_date_time_string, zero_time) {
	// hour cannot be checked due to local time differences
	const std::string format = "%Y_%m_%d_%M_%S";
	time_t time = 1574941523;
	const std::string time_string = "2019_11_28_45_23";
	auto result = utils::create_date_time_string(time, format);
	EXPECT_EQ(time_string, result);
}

TEST(string_to_vector, valid_string) {
	const std::string with_spaces = WORD_1 + " with spaces";
	auto word_list = WORD_1 + COMMA_SPACE + WORD_2 + COMMA_SPACE + with_spaces;
	auto vector = utils::string_to_vector(word_list);
	EXPECT_LE(5, vector.size());
	EXPECT_EQ(WORD_1, vector[0]);
	EXPECT_EQ(WORD_2, vector[1]);
	EXPECT_EQ(WORD_1, vector[2]);
}

TEST(string_to_pair_array, valid_string) {
	const std::string value_1 = "value_1";
	const std::string value_2 = "value_2_";
	const std::string value_3 = "value_3__";
	char separator = ';';
	auto pair_string =
		WORD_1 + separator + value_1 + SPACE +
		WORD_2 + separator + value_2 + SPACE +
		WORD_3 + separator + value_3;

	auto result = utils::string_to_pair_array(separator, pair_string);
	EXPECT_EQ(3, result.size());
	EXPECT_EQ(std::make_pair(WORD_1, value_1), result[0]);
	EXPECT_EQ(std::make_pair(WORD_2, value_2), result[1]);
	EXPECT_EQ(std::make_pair(WORD_3, value_3), result[2]);
}

TEST(string_to_pair_array, empty_values) {
	char separator = ';';
	auto pair_string =
		WORD_1 + separator + SPACE +
		WORD_2 + separator + SPACE +
		WORD_3 + separator;

	auto result = utils::string_to_pair_array(separator, pair_string);
	EXPECT_EQ(3, result.size());
	EXPECT_EQ(std::make_pair(WORD_1, EMPTY_STRING), result[0]);
	EXPECT_EQ(std::make_pair(WORD_2, EMPTY_STRING), result[1]);
	EXPECT_EQ(std::make_pair(WORD_3, EMPTY_STRING), result[2]);
}

TEST(decode_log_level, not_existing_log_level) {
	Log_level decoded = utils::decode_log_level(EMPTY_STRING);
	int decoded_value = static_cast<int>(decoded);
	EXPECT_GT(0, decoded_value);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
