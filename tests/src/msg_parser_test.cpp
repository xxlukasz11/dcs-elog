#include "gtest/gtest.h"
#include "msg_parser.h"
#include "message.h"

TEST(msg_parser, select_mode)
{
	Msg_parser parser("[1]");
	int mode = parser.get_mode();
	Message::Type type = Message::int_to_message_type(mode);
	EXPECT_EQ(type, Message::Type::return_events);
}

TEST(msg_parser, insert_mode) {
	Msg_parser parser("[0]");
	int mode = parser.get_mode();
	Message::Type type = Message::int_to_message_type(mode);
	EXPECT_EQ(type, Message::Type::create_event);
}

TEST(msg_parser, return_tags_tree_mode) {
	Msg_parser parser("[2]");
	int mode = parser.get_mode();
	Message::Type type = Message::int_to_message_type(mode);
	EXPECT_EQ(type, Message::Type::return_tags_tree);
}

TEST(msg_parser, add_tag_mode) {
	Msg_parser parser("[3]");
	int mode = parser.get_mode();
	Message::Type type = Message::int_to_message_type(mode);
	EXPECT_EQ(type, Message::Type::create_tag);
}

TEST(msg_parser, delete_tag_mode) {
	Msg_parser parser("[4]");
	int mode = parser.get_mode();
	Message::Type type = Message::int_to_message_type(mode);
	EXPECT_EQ(type, Message::Type::delete_tag);
}

TEST(msg_parser, update_tag_mode) {
	Msg_parser parser("[5]");
	int mode = parser.get_mode();
	Message::Type type = Message::int_to_message_type(mode);
	EXPECT_EQ(type, Message::Type::update_tag);
}

int main(int argc, char** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
