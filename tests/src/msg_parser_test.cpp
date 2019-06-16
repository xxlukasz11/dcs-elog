#include "gtest/gtest.h"
#include "msg_parser.h"

TEST(msg_parser, select_mode)
{
	Msg_parser parser("[1]");
	Msg_parser::mode mode = parser.get_mode();
	
	EXPECT_EQ(mode, Msg_parser::mode::select);	
}

TEST(msg_parser, insert_mode) {
	Msg_parser parser("[0]");
	Msg_parser::mode mode = parser.get_mode();

	EXPECT_EQ(mode, Msg_parser::mode::insert);
}

TEST(msg_parser, return_tags_tree_mode) {
	Msg_parser parser("[2]");
	Msg_parser::mode mode = parser.get_mode();

	EXPECT_EQ(mode, Msg_parser::mode::return_tags_tree);
}

int main(int argc, char** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
