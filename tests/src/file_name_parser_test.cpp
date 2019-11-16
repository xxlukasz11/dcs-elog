#include "gtest/gtest.h"
#include "file_name_parser.h"
#include <string>

const std::string EMPTY_STRING = "";
const std::string PATH = "dir/etc/";
const std::string PATH_WITH_DOTS = "dir.h/etc/.dummy/";
const std::string FILE_NAME = "file";
const std::string EXTENSION = ".jpg";
const std::string DOT = ".";

static File_name_parser parser;

TEST(file_name_parser, valid_file_name) {
	bool valid = parser.parse(FILE_NAME + EXTENSION);

	ASSERT_EQ(valid, true);
	ASSERT_EQ(parser.get_name(), FILE_NAME);
	ASSERT_EQ(parser.get_extension(), EXTENSION);
}

TEST(file_name_parser, valid_only_extension) {
	bool valid = parser.parse(EXTENSION);

	ASSERT_EQ(valid, true);
	ASSERT_EQ(parser.get_name(), EMPTY_STRING);
	ASSERT_EQ(parser.get_extension(), EXTENSION);
}

TEST(file_name_parser, valid_path_only_extension) {
	bool valid = parser.parse(PATH_WITH_DOTS + EXTENSION);

	ASSERT_EQ(valid, true);
	ASSERT_EQ(parser.get_name(), EMPTY_STRING);
	ASSERT_EQ(parser.get_extension(), EXTENSION);
}

TEST(file_name_parser, valid_path) {
	bool valid = parser.parse(PATH + FILE_NAME + EXTENSION);

	ASSERT_EQ(valid, true);
	ASSERT_EQ(parser.get_name(), FILE_NAME);
	ASSERT_EQ(parser.get_extension(), EXTENSION);
}

TEST(file_name_parser, valid_path_with_dots) {
	bool valid = parser.parse(PATH_WITH_DOTS + FILE_NAME + EXTENSION);

	ASSERT_EQ(valid, true);
	ASSERT_EQ(parser.get_name(), FILE_NAME);
	ASSERT_EQ(parser.get_extension(), EXTENSION);
}

TEST(file_name_parser, valid_file_name_no_extension) {
	bool valid = parser.parse(PATH_WITH_DOTS + FILE_NAME);

	ASSERT_EQ(valid, true);
	ASSERT_EQ(parser.get_name(), FILE_NAME);
	ASSERT_EQ(parser.get_extension(), EMPTY_STRING);
}

TEST(file_name_parser, invalid_only_directory) {
	bool valid = parser.parse(PATH_WITH_DOTS);

	ASSERT_EQ(valid, false);
	ASSERT_EQ(parser.get_name(), EMPTY_STRING);
	ASSERT_EQ(parser.get_extension(), EMPTY_STRING);
}


TEST(file_name_parser, invalid_empty_string) {
	bool valid = parser.parse(EMPTY_STRING);

	ASSERT_EQ(valid, false);
	ASSERT_EQ(parser.get_name(), EMPTY_STRING);
	ASSERT_EQ(parser.get_extension(), EMPTY_STRING);
}

TEST(file_name_parser, invalid_dot_at_the_end) {
	bool valid = parser.parse(FILE_NAME + DOT);

	ASSERT_EQ(valid, false);
	ASSERT_EQ(parser.get_name(), EMPTY_STRING);
	ASSERT_EQ(parser.get_extension(), EMPTY_STRING);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
