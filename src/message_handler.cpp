#include "custom_exceptions.h"
#include "return_events_procedure.h"
#include "return_tags_tree_procedure.h"
#include "create_tag_procedure.h"
#include "update_tag_procedure.h"
#include "delete_tag_procedure.h"
#include "create_event_procedure.h"
#include "update_event_procedure.h"
#include "message_handler.h"
#include "logger.h"

Message_handler::Message_handler(Socket socket, Database& database) : socket_(socket), database_(database) {}

void Message_handler::handle(const std::shared_ptr<Message>& message) {
	std::unique_ptr<Procedure> procedure = create_procedure(message);
	Logger::create().context(socket_).info("Starting " + procedure->name());
	procedure->start();
	Logger::create().context(socket_).info(procedure->name() + " has finished");
}

std::unique_ptr<Procedure> Message_handler::create_procedure(const std::shared_ptr<Message>& message) {
	using T = Message::Type;
	auto message_type = message->get_message_type();

	switch (message_type) {
		case T::create_event:
			return std::make_unique<Create_event_procedure>(database_, socket_, message);
		case T::return_events:
			return std::make_unique<Return_events_procedure>(database_, socket_, message);
		case T::return_tags_tree:
			return std::make_unique<Return_tags_tree_procedure>(database_, socket_, message);
		case T::create_tag:
			return std::make_unique<Create_tag_procedure>(database_, socket_, message);
		case T::delete_tag:
			return std::make_unique<Delete_tag_procedure>(database_, socket_, message);
		case T::update_tag:
			return std::make_unique<Update_tag_procedure>(database_, socket_, message);
		case T::update_event:
			return std::make_unique<Update_event_procedure>(database_, socket_, message);

		default: throw Unknown_message("Message with id = ", static_cast<int>(message_type), " is unknown");
	}
}
