#include "gtest/gtest.h"
#include "select_query.h"
#include "prepared_statement.h"
#include <vector>
#include <string>

const std::string TAG_1 = "a";
const std::string TAG_2 = "bcd";
const std::string MIN_DATE = "2019-06-01";
const std::string MAX_DATE = "2019-06-16";
const std::string COMMA_SPACE = ", ";

Select_query create_query() {
	Select_query query;
	query.create_tag(TAG_1);
	query.create_tag(TAG_2);
	query.set_min_date(MIN_DATE);
	query.set_max_date(MAX_DATE);
	return query;
}

TEST(select_query, print_sql_string) {
	Select_query query = create_query();
	Prepared_statement p = query.create_statement();
	std::string stmt = p.get_sql();

	EXPECT_TRUE(stmt.find("OR") != std::string::npos);
	EXPECT_TRUE(stmt.find("AND") != std::string::npos);
}

TEST(select_attachments_query, valid_event_id_section) {
	Select_query query = create_query();
	std::vector<std::string> event_id_list = { "1", "2", "11" };
	Prepared_statement stmt = query.create_attachments_statement(event_id_list);
	std::string sql = stmt.get_sql();

	const std::string expected = "(" +
		Prepared_statement::PARAM_PLACEHOLDER + COMMA_SPACE +
		Prepared_statement::PARAM_PLACEHOLDER + COMMA_SPACE +
		Prepared_statement::PARAM_PLACEHOLDER + ")";
	EXPECT_TRUE(stmt.get_sql().find(expected) != std::string::npos);
	EXPECT_EQ(event_id_list, stmt.get_params());
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}