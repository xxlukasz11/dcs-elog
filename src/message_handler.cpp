#include "custom_exceptions.h"
#include "return_events_procedure.h"
#include "return_tags_tree_procedure.h"
#include "create_tag_procedure.h"
#include "update_tag_procedure.h"
#include "delete_tag_procedure.h"
#include "create_event_procedure.h"
#include "update_event_procedure.h"
#include "create_library_event_procedure.h"
#include "message_handler.h"
#include "logger.h"
#include "message_factory.h"

Message_handler::Message_handler(Socket socket, Database& database) : socket_(socket), database_(database), attachment_handler_rx_(socket) {}

void Message_handler::process_message(const std::string& message_string) {
	try {
		Message_factory factory(message_string);
		auto internal_message = factory.create();
		Logger::create().context(socket_).level(Log_level::INFO).info(internal_message);
		handle(internal_message);

	} catch (const Unknown_message_format& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	} catch (const Unknown_message& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	} catch (const Database_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	} catch (const Query_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	} catch (const Send_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	} catch (const Attachment_error& e) {
		Logger::create().context(socket_).level(Log_level::WARNING).error(e.what());
	}
}

void Message_handler::handle(const std::shared_ptr<Message>& message) {
	std::unique_ptr<Procedure> procedure = create_procedure(message);
	Logger::create().context(socket_).level(Log_level::INFO).info("Starting " + procedure->name());
	procedure->start();
	Logger::create().context(socket_).level(Log_level::INFO).info(procedure->name() + " has finished");
}

std::unique_ptr<Procedure> Message_handler::create_procedure(const std::shared_ptr<Message>& message) {
	using T = Message::Type;
	auto message_type = message->get_message_type();

	switch (message_type) {
		case T::create_event:
			return create_procedure<Create_event_procedure>(message);
		case T::return_events:
			return create_procedure<Return_events_procedure>(message);
		case T::return_tags_tree:
			return create_procedure<Return_tags_tree_procedure>(message);
		case T::create_tag:
			return create_procedure<Create_tag_procedure>(message);
		case T::delete_tag:
			return create_procedure<Delete_tag_procedure>(message);
		case T::update_tag:
			return create_procedure<Update_tag_procedure>(message);
		case T::update_event:
			return create_procedure<Update_event_procedure>(message);
		case T::create_library_event:
			return create_procedure<Create_library_event_procedure>(message);

		default: throw Unknown_message("Message with id = ", static_cast<int>(message_type), " is unknown");
	}
}
