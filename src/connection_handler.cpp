#include "database.h"
#include "result_set.h"
#include "custom_exceptions.h"
#include "json.h"
#include "utils.h"
#include "config.h"

#include "insert_query.h"
#include "select_query.h"
#include "select_tags_query.h"
#include "add_tag_query.h"
#include "delete_tag_query.h"
#include "update_tag_query.h"
#include "update_event_query.h"

#include "connection_handler.h"

std::mutex Connection_handler::mtx_;

Connection_handler::Connection_handler(Socket socket) : socket_(socket) {}

void Connection_handler::handle_message(const Return_events_request& message) {
	auto min_date_str = message.get_min_date();
	auto max_date_str = message.get_max_date();
	auto tags_str = message.get_tags();

	Select_query query;
	if (!min_date_str.empty()) {
		query.set_min_date(min_date_str);
	}
	if (!max_date_str.empty()) {
		query.set_max_date(max_date_str);
	}

	auto extracted_tags = utils::string_to_vector(tags_str);
	if (!extracted_tags.empty()) {
		query.set_tags(std::move(extracted_tags));
	}

	Result_set res;
	{
		std::lock_guard<std::mutex> lock(mtx_);
		Database db(config::path::database);
		db.open();
		auto stmt = query.create_statement();
		res = db.execute(stmt);
	}

	socket_.send_string(json::stringify(std::move(res)));
}

void Connection_handler::handle_message(const Create_event_request& message) {
	Insert_query query;
	query.set_title(message.get_title());
	query.set_desc(message.get_description());
	query.set_tags(utils::string_to_vector(message.get_tags()));
	query.set_author(message.get_author());

	bool tags_exist = true;
	Result_set not_existing_tags;
	{
		Prepared_statement events_stmt = query.create_events_statement();
		Prepared_statement exists_stmt = query.create_tags_exist_statement();

		std::lock_guard<std::mutex> lock(mtx_);

		Database db(config::path::database);
		db.open();

		not_existing_tags = db.execute(exists_stmt);
		if (not_existing_tags.get_data().size() != 0) {
			tags_exist = false;
		}
		else {
			Result_set res = db.execute(events_stmt);
			std::string last_id = res.get_last_row_id();
			db.execute(query.create_tags_statement(last_id));
		}
	}

	if (tags_exist) {
		socket_.send_string("Event has been successfully saved");
	}
	else {
		std::string message = "Cannot save event. Following tags do not exist: ";
		message += utils::concatenate_string_array(not_existing_tags.get_column(0));
		socket_.send_string(message);
	}
}

void Connection_handler::handle_message(const Return_tags_tree_request& message) {
	Result_set res;
	{
		Select_tags_query query;
		auto stmt = query.create_sql();

		std::lock_guard<std::mutex> lock(mtx_);
		Database db(config::path::database);
		db.open();
		res = db.execute(stmt);
	}

	socket_.send_string(json::stringify(std::move(res)));
}

void Connection_handler::handle_message(const Create_tag_request& message) {
	Add_tag_query query;
	query.set_tag_name(message.get_tag_name());
	query.set_parent_id(message.get_parent_id());

	{
		std::lock_guard<std::mutex> lock(mtx_);

		Database db(config::path::database);
		db.open();

		Result_set res = db.execute(query.create_tag_statement());
		std::string last_id = res.get_last_row_id();
		db.execute(query.create_tree_statement(last_id));
	}

	socket_.send_string("Tag '" + query.get_tag_name() + "' has been created");
}

void Connection_handler::handle_message(const Delete_tag_request& message) {
	Delete_tag_query query;
	query.set_tag_id(message.get_tag_id());

	Prepared_statement select_stmt = query.select_statement();
	Prepared_statement parent_id_null_stmt = query.parent_id_statement(Delete_tag_query::ALLOW_NULL);
	Prepared_statement parent_id_default_stmt = query.parent_id_statement(Delete_tag_query::RETURN_DEFAULT);
	Prepared_statement delete_redundant_stmt = query.delete_redundant_statement();
	Prepared_statement delete_tree_stmt = query.delete_tree_statement();
	Prepared_statement delete_list_stmt = query.delete_list_statement();

	bool tag_exists = false;
	bool empty_tag_delete_attempt = false;
	std::string tag_name;
	{
		std::lock_guard<std::mutex> lock(mtx_);
		Database db(config::path::database);
		db.open();

		Result_set res = db.execute(select_stmt);
		tag_exists = res.get_data().size() > 0;

		if (tag_exists) {
			tag_name = res.get_data()[0][0];

			if (tag_name == config::symbols::empty_tag) {
				empty_tag_delete_attempt = true;
			}
		}
			
		if(tag_exists && !empty_tag_delete_attempt) {
			Result_set res = db.execute(parent_id_null_stmt);
			std::string parent_id = res.get_data()[0][0];

			Result_set res_events = db.execute(parent_id_default_stmt);
			std::string parent_id_for_events = res_events.get_data()[0][0];

			Prepared_statement delete_events_tag_stmt = query.delete_events_tag_statement(parent_id_for_events);
			Prepared_statement update_events_tag_stmt = query.update_events_tag_statement(parent_id_for_events);
			Prepared_statement update_tree_stmt = query.update_tree_statement(parent_id);

			db.execute(delete_events_tag_stmt);
			db.execute(update_events_tag_stmt);
			db.execute(delete_redundant_stmt);
			db.execute(update_tree_stmt);
			db.execute(delete_tree_stmt);
			db.execute(delete_list_stmt);
		}

	}

	if (tag_exists) {
		if(empty_tag_delete_attempt)
			socket_.send_string("Tag '" + config::symbols::empty_tag + "' cannot be deleted");
		else
			socket_.send_string("Tag '" + tag_name + "' has been deleted");
	}
	else {
		socket_.send_string("Tag with id: " + query.get_tag_id() + " has already been deleted");
	}
	
}

void Connection_handler::handle_message(const Update_tag_request& message) {
	Update_tag_query query;
	query.set_tag_id(message.get_tag_id());
	query.set_tag_name(message.get_new_name());

	Prepared_statement select_stmt = query.create_select_tag_statement();
	Prepared_statement update_stmt = query.create_update_statement();

	auto execute = [&select_stmt, &update_stmt, &query]() -> std::string {
		std::lock_guard<std::mutex> lock(mtx_);
		Database db(config::path::database);
		db.open();

		Result_set res = db.execute(select_stmt);
		bool tag_exists = res.get_data().size() > 0;

		if (tag_exists) {
			std::string old_tag_name = res.get_data()[0][0];
			if (old_tag_name != config::symbols::empty_tag) {
				db.execute(update_stmt);
				return "Tag name has been changed from '" + old_tag_name + "' to '" + query.get_tag_name() + "'";
			}
			else {
				return "Cannot update reserved tag: '" + config::symbols::empty_tag + "'";
			}
		}
		else {
			return "Cannot update tag. It does not exist.";
		}
	};

	std::string status = execute();
	socket_.send_string(status);
}

void Connection_handler::handle_message(const Update_event_request& message) {
	auto id = message.get_event_id();
	auto title = message.get_title();
	auto description = message.get_description();
	auto tags = message.get_tags();

	Insert_query insert_query;
	insert_query.set_tags(utils::string_to_vector(tags));

	Update_event_query update_query;
	update_query.set_id(id);
	update_query.set_title(title);
	update_query.set_tags(utils::string_to_vector(tags));
	update_query.set_description(description);

	Prepared_statement event_exists_stmt = update_query.create_event_exists_stmt();
	Prepared_statement tags_exists_stmt = insert_query.create_tags_exist_statement();
	Prepared_statement update_event_stmt = update_query.create_update_event_stmt();
	Prepared_statement delete_event_tags_stmt = update_query.create_delete_event_tags_statement();
	Prepared_statement insert_event_tags_stmt = insert_query.create_tags_statement(id);

	auto execute = [&]() -> std::string {
		std::lock_guard<std::mutex> lock(mtx_);
		Database db(config::path::database);
		db.open();

		auto event_exists_result = db.execute(event_exists_stmt);
		if (event_exists_result.get_data().size() == 0) {
			return "Cannot update event with id: " + id + ". It does not exist";
		}

		auto not_existing_tags = db.execute(tags_exists_stmt);
		if (not_existing_tags.get_data().size() != 0) {
			std::string message = "Cannot update event. Following tags do not exist: ";
			message += utils::concatenate_string_array(not_existing_tags.get_column(0));
			return message;
		}

		db.execute(update_event_stmt);
		db.execute(delete_event_tags_stmt);

		if (update_query.has_any_tags()) {
			db.execute(insert_event_tags_stmt);
		}

		std::string event_string = "Event ";
		return event_string + id + " has been successfully updated";
	};

	std::string status = execute();
	utils::out_log(socket_, status);
	socket_.send_string(status);
}

void Connection_handler::handle(const std::shared_ptr<Message>& message) {
	using T = Message::Type;
	auto message_type = message->get_message_type();

	switch (message_type) {
		case T::create_event:		handle_message(dynamic_cast<Create_event_request&>(*message));	break;
		case T::return_events:		handle_message(dynamic_cast<Return_events_request&>(*message)); break;
		case T::return_tags_tree:	handle_message(dynamic_cast<Return_tags_tree_request&>(*message));	break;
		case T::create_tag:			handle_message(dynamic_cast<Create_tag_request&>(*message));	break;
		case T::delete_tag:			handle_message(dynamic_cast<Delete_tag_request&>(*message));	break;
		case T::update_tag:			handle_message(dynamic_cast<Update_tag_request&>(*message));	break;
		case T::update_event:		handle_message(dynamic_cast<Update_event_request&>(*message));	break;

		default: throw Unknown_message("Message with id = ", static_cast<int>(message_type), " is unknown");
	}
}