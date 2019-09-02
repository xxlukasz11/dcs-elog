#include "gtest/gtest.h"
#include "msg_parser.h"

TEST(msg_parser, select_mode)
{
	Msg_parser parser("[1]");
	Msg_parser::mode mode = parser.get_mode();
	EXPECT_EQ(mode, Msg_parser::mode::return_events);	
}

TEST(msg_parser, insert_mode) {
	Msg_parser parser("[0]");
	Msg_parser::mode mode = parser.get_mode();
	EXPECT_EQ(mode, Msg_parser::mode::add_event);
}

TEST(msg_parser, return_tags_tree_mode) {
	Msg_parser parser("[2]");
	Msg_parser::mode mode = parser.get_mode();
	EXPECT_EQ(mode, Msg_parser::mode::return_tags_tree);
}

TEST(msg_parser, add_tag_mode) {
	Msg_parser parser("[3]");
	Msg_parser::mode mode = parser.get_mode();
	EXPECT_EQ(mode, Msg_parser::mode::add_tag);
}

TEST(msg_parser, delete_tag_mode) {
	Msg_parser parser("[4]");
	Msg_parser::mode mode = parser.get_mode();
	EXPECT_EQ(mode, Msg_parser::mode::delete_tag);
}

TEST(msg_parser, update_tag_mode) {
	Msg_parser parser("[5]");
	Msg_parser::mode mode = parser.get_mode();
	EXPECT_EQ(mode, Msg_parser::mode::update_tag);
}

int main(int argc, char** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
