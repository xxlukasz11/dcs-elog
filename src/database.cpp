#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "custom_exceptions.h"
#include "result_set.h"
#include "sqlite3.h"

#include "database.h"

Database::Database(std::string path) : path_(path) {}

Database::~Database(){}

void Database::open(){
	sqlite3* db;
	if (sqlite3_open(path_.c_str(), &db) != SQLITE_OK){
		throw Database_error("Cant open database: ", path_);
	}
	handler_ = std::unique_ptr<sqlite3, Database::deleter_type>(db, [](sqlite3* ptr){
		sqlite3_close(ptr);
	});
}

void Database::close() {
	if (handler_) {
		sqlite3* db = handler_.release();
		sqlite3_close(db);
	}
}

void Database::assert_database_opened() {
	if (!is_database_opened()) {
		throw Database_error("Error: Database is not opened");
	}
}

bool Database::is_database_opened() const {
	if (handler_) {
		return true;
	}
	return false;
}

void Database::execute(const std::string& query, Database::callback_type callback){
	assert_database_opened();
	char *err_msg;
	if( sqlite3_exec(handler_.get(), query.c_str(), callback, 0, &err_msg) != SQLITE_OK ){
		throw Database_error("Error executing statement: ", err_msg);
	}
}

void Database::execute(const std::string& query, Database::user_callback_type callback){
	callback( execute( query ) );
}

Result_set Database::execute(const std::string& query){
	assert_database_opened();
	Result_set data;
	char *err_msg;

	int err = sqlite3_exec(handler_.get(), query.c_str(), [](void* ptr, int num_of_columns, char** row, char** columns){
		Result_set& inner_data = *(reinterpret_cast<Result_set*>(ptr));

		if(inner_data.is_header_empty())
			inner_data.set_header(columns, num_of_columns);
		inner_data.insert_row(row, num_of_columns);

		return 0;
	}, &data, &err_msg);

	if( err != SQLITE_OK ){
		throw Database_error("Error executing statement: ", err_msg);
	}

	data.set_last_row_id( get_last_row_id() );
	return data;
}

Result_set Database::execute(const Prepared_statement& statement){
	sqlite3_stmt* stmt = 0;
	int err;

	if( (err = sqlite3_prepare_v2( handler_.get(), statement.get_sql().c_str(), -1, &stmt, 0 )) != SQLITE_OK )
		throw Database_error("Error executing statement: Cannot prepare statement");

	bind_params(stmt, statement.get_params());

	Result_set data;
	while ( sqlite3_step( stmt ) == SQLITE_ROW ){
		if(data.is_header_empty())
			set_header(stmt, data);
		insert_data(stmt, data);
	}

	if( sqlite3_finalize(stmt) != SQLITE_OK )
		throw Database_error("Error executing statement: Cannot finalize statement");

	data.set_last_row_id( get_last_row_id() );
	return data;
}

void Database::bind_params(sqlite3_stmt* stmt, const Prepared_statement::params_type& params){
	for(size_t i = 0; i < params.size(); ++i){
		int err;
		if (params[i].size() == 0)
			err = sqlite3_bind_null(stmt, i+1);
		else
			err = sqlite3_bind_text(stmt, i+1, params[i].c_str(), -1, SQLITE_TRANSIENT);
		if(err != SQLITE_OK)
			throw Database_error("Error executing statement: Cannot bind parameters");
	}
}

void Database::insert_data(sqlite3_stmt* stmt, Result_set& result_set){
	auto n = sqlite3_column_count( stmt );
	std::vector<std::string> row(n);

	for ( int i = 0; i < n; i++ ) {
		auto* col = sqlite3_column_text(stmt, i);
		if(col != nullptr)
			row[i] = (const char*)col;
		else
			row[i] = std::string();
	}
	result_set.insert_row(std::move(row));
}

void Database::set_header(sqlite3_stmt* stmt, Result_set& result_set){
	auto n = sqlite3_column_count( stmt );
	std::vector<std::string> h(n);

	for(int i = 0; i < n; ++i){
		auto* col_name = sqlite3_column_name(stmt, i);
		if(col_name != nullptr)
			h[i] = (const char*)col_name;
		else
			h[i] = std::string();
	}
	result_set.set_header(std::move(h));
}

Result_set Database::execute(const std::vector<Prepared_statement>& statements){
	Result_set main_result;
	for(const auto& stmt : statements){
		auto tmp_res = execute(stmt);
		main_result += std::move(tmp_res);
	}
	return main_result;
}

std::string Database::get_last_row_id(){
	std::string row_id;

	sqlite3_exec(handler_.get(), "SELECT last_insert_rowid();", [](void* ptr, int num_of_columns, char** row, char** columns){
		std::string& id = *(reinterpret_cast<std::string*>(ptr));
		id = row[0];
		return 0;
	}, &row_id, 0);

	return row_id;
}

Database::Accessor::Accessor(Database& database) : database_(database), locker_(database.mtx_, std::defer_lock) {
}

void Database::Accessor::open() {
	locker_.lock();
	database_.open();
}

void Database::Accessor::close() {
	if (locker_.owns_lock()) {
		database_.close();
		locker_.unlock();
	}
}

Database::Accessor::~Accessor() {
	close();
}
