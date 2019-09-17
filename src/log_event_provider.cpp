#include <utility>
#include <memory>
#include "log_event_provider.h"

Log_event_provider::Log_event_provider(Log_item_queue& queue) : queue_(queue) {
}

Log_event_provider& Log_event_provider::context(int id) {
	log_item_.set_context(id);
	return *this;
}

Log_event_provider& Log_event_provider::location(const std::string& location) {
	log_item_.set_location(location);
	return *this;
}

void Log_event_provider::info(const std::string& message) {
	log_item_.set_message(Log_item::Type::INFO, message);
	push_to_queue();
}

void Log_event_provider::info(const std::shared_ptr<Message>& message) {
	std::string message_string = "Recieved " + message->name();
	log_item_.set_message(Log_item::Type::INFO, message_string);
	push_to_queue();
}

void Log_event_provider::warning(const std::string& message) {
	log_item_.set_message(Log_item::Type::WARNING, message);
	push_to_queue();
}

void Log_event_provider::error(const std::string& message) {
	log_item_.set_message(Log_item::Type::ERROR, message);
	push_to_queue();
}

void Log_event_provider::status(const std::string& message) {
	log_item_.set_message(Log_item::Type::STATUS, message);
	push_to_queue();
}

void Log_event_provider::push_to_queue() {
	queue_.push(std::move(log_item_));
}
