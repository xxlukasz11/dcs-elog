#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <mutex>

#include "sqlite3.h"
#include "prepared_statement.h"
#include "result_set.h"

class Database{
	using deleter_type = std::function<void(sqlite3*)>;
public:
	class Accessor {
		Database& database_;
		std::unique_lock<std::mutex> locker_;
	public:
		Accessor(Database& database);
		void open();
		void close();
		~Accessor();
	};

	using callback_type = int(*)(void*, int, char**, char**);
	using user_callback_type = std::function<void(Result_set&& result)>;
	using response_type = std::vector<std::vector<std::string>>;
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;
	Database(std::string path);
	~Database();

	void execute(const std::string& query, callback_type callback);
	void execute(const std::string& query, user_callback_type callback);
	Result_set execute(const std::string& query);
	Result_set execute(const Prepared_statement& statement);
	Result_set execute(const std::vector<Prepared_statement>& statements);
	
private:
	std::string get_last_row_id();

	void open();
	void close();
	void assert_database_opened();
	bool is_database_opened() const;
	void bind_params(sqlite3_stmt* stmt, const Prepared_statement::params_type& params);
	void insert_data(sqlite3_stmt* stmt, Result_set& result_set);
	void set_header(sqlite3_stmt* stmt, Result_set& result_set);

	std::unique_ptr<sqlite3, deleter_type> handler_;
	std::string path_;
	std::mutex mtx_;
};

#endif