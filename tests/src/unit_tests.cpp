#include "gtest/gtest.h"
#include "msg_parser.h"

TEST(msg_parser, mode_test)
{
	Msg_parser parser("[1]");
	Msg_parser::mode mode = parser.get_mode();

    EXPECT_EQ(mode, Msg_parser::mode::select);	
}

int main(int argc, char** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
