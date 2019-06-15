#include <iostream>
#include "gtest/gtest.h"
#include "select_query.h"
#include "prepared_statement.h"

TEST(select_query, sql_string) {
	Select_query query;
	query.add_tag("a");
	query.add_tag("b");
	query.set_min_date("2019-06-01");
	query.set_max_date("2019-06-16");
	
	Prepared_statement p = query.create_statement();
	std::cout << p.get_sql() << std::endl;
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}