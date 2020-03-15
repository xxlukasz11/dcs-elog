#include "gtest/gtest.h"
#include "select_query.h"
#include "prepared_statement.h"
#include <vector>
#include <string>

const std::string TAG_1 = "a";
const std::string TAG_2 = "bcd";
const std::string MIN_DATE = "2019-06-01";
const std::string MAX_DATE = "2019-06-16";
const std::string LIMIT = "15";
const std::string OFFSET = "52";
const std::string COMMA_SPACE = ", ";

Select_query create_query() {
	Select_query query;
	query.create_tag(TAG_1);
	query.create_tag(TAG_2);
	query.set_min_date(MIN_DATE);
	query.set_max_date(MAX_DATE);
	query.set_limit(LIMIT);
	query.set_offset(OFFSET);
	return query;
}

Prepared_statement create_return_events_statement() {
	Select_query query = create_query();
	return query.create_statement();
}

void assert_string_contains(const std::string string, const std::string match) {
	const auto contains = string.find(match);
	EXPECT_TRUE(contains != std::string::npos);
}

void assert_not_string_contains(const std::string string, const std::string match) {
	const auto contains = string.find(match);
	EXPECT_TRUE(contains == std::string::npos);
}

TEST(select_query_test, has_limit_set) {
	Select_query query;
	query.set_limit(LIMIT);
	const std::string sql = query.create_statement().get_sql();
	assert_string_contains(sql, "LIMIT " + Prepared_statement::PARAM_PLACEHOLDER);
}

TEST(select_query_test, has_offset_set) {
	Select_query query;
	query.set_offset(OFFSET);
	const std::string sql = query.create_statement().get_sql();
	assert_string_contains(sql, "OFFSET " + Prepared_statement::PARAM_PLACEHOLDER);
}

TEST(select_query_test, has_not_limit_set) {
	Select_query query;
	query.set_offset(OFFSET);
	const std::string sql = query.create_statement().get_sql();
	assert_not_string_contains(sql, "LIMIT " + Prepared_statement::PARAM_PLACEHOLDER);
}

TEST(select_query_test, has_not_offset_set) {
	Select_query query;
	query.set_limit(LIMIT);
	const std::string sql = query.create_statement().get_sql();
	assert_not_string_contains(sql, "OFFSET " + Prepared_statement::PARAM_PLACEHOLDER);
}

TEST(select_query_test, sql_contains_logic_operators) {
	Prepared_statement stmt = create_return_events_statement();
	std::string sql = stmt.get_sql();
	assert_string_contains(sql, "OR");
	assert_string_contains(sql, "AND");
}

TEST(select_query_test, valid_event_id_section_in_attachment_stmt) {
	Select_query query = create_query();
	std::vector<std::string> event_id_list = { "1", "2", "11" };
	Prepared_statement stmt = query.create_attachments_statement(event_id_list);

	const std::string expected = "(" +
		Prepared_statement::PARAM_PLACEHOLDER + COMMA_SPACE +
		Prepared_statement::PARAM_PLACEHOLDER + COMMA_SPACE +
		Prepared_statement::PARAM_PLACEHOLDER + ")";
	assert_string_contains(stmt.get_sql(), expected);
	EXPECT_EQ(event_id_list, stmt.get_params());
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
