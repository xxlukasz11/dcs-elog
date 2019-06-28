#include "connection_handler.h"

#include "database.h"
#include "result_set.h"
#include "custom_exceptions.h"
#include "json.h"

#include "insert_query.h"
#include "select_query.h"
#include "select_tags_query.h"
#include "add_tag_query.h"
#include "delete_tag_query.h"

std::mutex Connection_handler::mtx_;

Connection_handler::Connection_handler(Msg_parser& parser, Socket socket) : parser_(parser), socket_(socket) {}

namespace {
	std::string concatenate_string_array(const std::vector<std::string>& array) {
		std::string text;
		size_t array_size = array.size();
		for (size_t i = 0; i < array_size; ++i) {
			text += array[i];
			if(i != array_size-1)
				text += ", ";
		}
		return text;
	}

}

template<>
void Connection_handler::handle<Msg_parser::mode::select>() {
	auto min_date_str = parser_.next();
	auto max_date_str = parser_.next();
	auto tags_str = parser_.next();

	Select_query query;
	if (!min_date_str.empty()) {
		query.set_min_date(min_date_str);
	}
	if (!max_date_str.empty()) {
		query.set_max_date(max_date_str);
	}

	auto extracted_tags = Msg_parser::extract_tags(tags_str);
	if (!extracted_tags.empty()) {
		query.set_tags(std::move(extracted_tags));
	}

	Result_set res;
	{
		std::lock_guard<std::mutex> lock(mtx_);
		Database db("../resources/database.db");
		db.open();
		auto stmt = query.create_statement();
		res = db.execute(stmt);
	}

	socket_.send_string(Json::stringify(std::move(res)));
}

template<>
void Connection_handler::handle<Msg_parser::mode::insert>() {
	Insert_query query;
	query.set_title(parser_.next());
	query.set_desc(parser_.next());
	query.set_tags(Msg_parser::extract_tags(parser_.next()));
	query.set_author(parser_.next());

	bool tags_exist = true;
	Result_set not_existing_tags;
	{
		Prepared_statement events_stmt = query.create_events_statement();
		Prepared_statement exists_stmt = query.create_tags_exist_statement();

		std::lock_guard<std::mutex> lock(mtx_);

		Database db("../resources/database.db");
		db.open();

		not_existing_tags = db.execute(exists_stmt);
		if (not_existing_tags.get_data().size() != 0) {
			tags_exist = false;
		}
		else {
			Result_set res = db.execute(events_stmt);
			std::string last_id = res.get_last_row_id();
			db.execute(query.create_tags_statements(last_id));
		}
	}

	if (tags_exist) {
		socket_.send_string("Event has been successfully saved");
	}
	else {
		std::string message = "Cannot save event. Following tags do not exist: ";
		message += concatenate_string_array(not_existing_tags.get_column(0));
		socket_.send_string(message);
	}
}

template<>
void Connection_handler::handle<Msg_parser::mode::return_tags_tree>() {
	Result_set res;
	{
		Select_tags_query query;
		auto stmt = query.create_sql();

		std::lock_guard<std::mutex> lock(mtx_);
		Database db("../resources/database.db");
		db.open();
		res = db.execute(stmt);
	}

	socket_.send_string(Json::stringify(std::move(res)));
}

template<>
void Connection_handler::handle<Msg_parser::mode::add_tag>() {
	Add_tag_query query;
	query.set_tag_name(parser_.next());
	query.set_parent_id(parser_.next());

	{
		std::lock_guard<std::mutex> lock(mtx_);

		Database db("../resources/database.db");
		db.open();

		Result_set res = db.execute(query.create_tag_statement());
		std::string last_id = res.get_last_row_id();
		db.execute(query.create_tree_statement(last_id));
	}

	socket_.send_string("Tag '" + query.get_tag_name() + "' has been added");
}

template<>
void Connection_handler::handle<Msg_parser::mode::delete_tag>() {
	Delete_tag_query query;
	query.set_tag_id(parser_.next());

	Prepared_statement select_stmt = query.create_select_statement();
	Prepared_statement tree_stmt = query.create_tree_statement();
	Prepared_statement list_stmt = query.create_list_statement();
	bool deleted = false;
	std::string tag_name;

	{
		std::lock_guard<std::mutex> lock(mtx_);

		Database db("../resources/database.db");
		db.open();

		Result_set res = db.execute(select_stmt);
		if (res.get_data().size() > 0) {
			deleted = true;
			tag_name = res.get_data()[0][0];
			db.execute(tree_stmt);
			db.execute(list_stmt);
		}

	}

	if (deleted) {
		socket_.send_string("Tag '" + tag_name + "' has been deleted");
	}
	else {
		socket_.send_string("Tag with id: " + query.get_tag_id() + " has already been deleted");
	}
}

void Connection_handler::handle() {
	using M = Msg_parser::mode;
	auto mode = parser_.get_mode();

	switch (mode) {
	case M::insert:				handle<M::insert>();			break;
	case M::select:				handle<M::select>();			break;
	case M::return_tags_tree:	handle<M::return_tags_tree>();	break;
	case M::add_tag:			handle<M::add_tag>();			break;
	case M::delete_tag:			handle<M::delete_tag>();		break;

	default: throw Unknown_message_format();
	}
}