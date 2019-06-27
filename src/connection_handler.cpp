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

	{
		std::lock_guard<std::mutex> lock(mtx_);

		Database db("../resources/database.db");
		db.open();

		Result_set res = db.execute(query.create_events_statement());
		std::string last_id = res.get_last_row_id();
		db.execute(query.create_tags_statements(last_id));
	}

	socket_.send_string("Event has been successfully saved");
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

	try {
		int parent_id = stoi(parser_.next());
		query.set_parent_id(parent_id);
	}
	catch (const std::invalid_argument& e) {
		throw Query_error("Cannot convert parent_id to integer value");
	}

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

	try {
		int tag_id = stoi(parser_.next());
		query.set_tag_id(tag_id);
	}
	catch (const std::invalid_argument& e) {
		throw Query_error("Cannot convert tag_id to integer value");
	}

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

	if(deleted)
		socket_.send_string("Tag '" + tag_name + "' has been deleted");
}

void Connection_handler::handle() {
	using M = Msg_parser::mode;
	auto mode = parser_.get_mode();

	switch (mode) {
		case M::insert :			handle<M::insert>();			break;
		case M::select :			handle<M::select>();			break;
		case M::return_tags_tree :	handle<M::return_tags_tree>();	break;
		case M::add_tag:			handle<M::add_tag>();			break;
		case M::delete_tag:			handle<M::delete_tag>();		break;

		default: throw Unknown_message_format();
	}
}
