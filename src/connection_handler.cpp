#include "connection_handler.h"

#include "database.h"
#include "result_set.h"
#include "custom_exceptions.h"
#include "json.h"

#include "insert_query.h"
#include "select_query.h"
#include "select_tags_query.h"

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

void Connection_handler::handle() {
	auto mode = parser_.get_mode();
	handle<Msg_parser::mode::return_tags_tree>();

	switch (mode) {
		case Msg_parser::mode::insert:			 handle<Msg_parser::mode::insert>();			break;
		case Msg_parser::mode::select:			 handle<Msg_parser::mode::select>();			break;
		case Msg_parser::mode::return_tags_tree: handle<Msg_parser::mode::return_tags_tree>();	break;

		default: throw Unknown_message_format();
	}
}
