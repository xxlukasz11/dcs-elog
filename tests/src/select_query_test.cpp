#include <iostream>
#include "gtest/gtest.h"
#include "select_query.h"
#include "prepared_statement.h"

TEST(select_query, print_sql_string) {
	Select_query query;
	query.create_tag("a");
	query.create_tag("b");
	query.set_min_date("2019-06-01");
	query.set_max_date("2019-06-16");
	
	Prepared_statement p = query.create_statement();
	std::string s = p.get_sql();
	EXPECT_TRUE(s.find("OR") != std::string::npos);
	EXPECT_TRUE(s.find("AND") != std::string::npos);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}