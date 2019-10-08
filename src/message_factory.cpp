#include "message_factory.h"
#include "msg_parser.h"
#include "custom_exceptions.h"

#include "return_events_request.h"
#include "return_tags_tree_request.h"
#include "create_event_request.h"
#include "create_tag_request.h"
#include "delete_tag_request.h"
#include "update_event_request.h"
#include "update_tag_request.h"
#include "create_library_event_request.h"

Message_factory::Message_factory(const std::string& message_string) : message_string_(message_string) {
}

std::shared_ptr<Message> Message_factory::create() const {
	Msg_parser parser(message_string_);
	auto message_type = extract_message_type(parser);
	std::shared_ptr<Message> message = create_message(message_type);
	read_message_contents(message, parser);
	return message;
}

Message::Type Message_factory::extract_message_type(Msg_parser& parser) const {
	try {
		parser.parse_header();
	} catch (const Msg_parser_exception& e) {
		throw Unknown_message(e.what());
	}
	int parser_mode = parser.get_mode();
	return Message::int_to_message_type(parser_mode);
}

std::shared_ptr<Message> Message_factory::create_message(Message::Type message_type) const {
	using T = Message::Type;
	switch (message_type) {
		case T::create_event: return std::make_shared<Create_event_request>();
		case T::return_events: return std::make_shared<Return_events_request>();
		case T::return_tags_tree: return std::make_shared<Return_tags_tree_request>();
		case T::create_tag: return std::make_shared<Create_tag_request>();
		case T::delete_tag: return std::make_shared<Delete_tag_request>();
		case T::update_tag: return std::make_shared<Update_tag_request>();
		case T::update_event: return std::make_shared<Update_event_request>();
		case T::create_library_event: return std::make_shared<Create_library_event_request>();

		default: throw Unknown_message("Message with id = ", static_cast<int>(message_type), " is unknown");
	}
}

void Message_factory::read_message_contents(const std::shared_ptr<Message>& message, Msg_parser& parser) const {
	try {
		message->extract_parameters(parser);
	} catch (const Msg_parser_exception& e) {
		throw Unknown_message_format(message->name());
	}
}
