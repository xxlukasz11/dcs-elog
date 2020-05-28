#include "gtest/gtest.h"
#include "concurrent_queue.h"

constexpr int ELEMENT_1 = 5;
constexpr int ELEMENT_2 = 14;
constexpr int NO_VALUE = 999;
constexpr auto TIMEOUT = std::chrono::milliseconds(1);

using IntQueue = Concurrent_queue<int>;

TEST(concurrent_queue_test, should_add_element) {
	IntQueue queue;
	queue.push(ELEMENT_1);
	auto actual = queue.pop();
	ASSERT_EQ(ELEMENT_1, actual);
}

TEST(concurrent_queue_test, should_store_elements_in_fifo_style) {
	IntQueue queue;
	queue.push(ELEMENT_1);
	queue.push(ELEMENT_2);
	const auto first = queue.pop();
	const auto second = queue.pop();
	ASSERT_EQ(ELEMENT_1, first);
	ASSERT_EQ(ELEMENT_2, second);
}

TEST(concurrent_queue_test, should_indicate_correct_emptyness) {
	IntQueue queue;
	ASSERT_TRUE(queue.is_empty());
	queue.push(ELEMENT_1);
	ASSERT_FALSE(queue.is_empty());
	queue.pop();
	ASSERT_TRUE(queue.is_empty());
}

TEST(concurrent_queue_test, should_fail_to_pop) {
	IntQueue queue;
	auto output = queue.try_pop(TIMEOUT);
	auto value = output.value_or(NO_VALUE);
	ASSERT_EQ(NO_VALUE, value);
}

TEST(concurrent_queue_test, should_pop_with_timeout) {
	IntQueue queue;
	queue.push(ELEMENT_1);
	auto output = queue.try_pop(TIMEOUT);
	auto value = output.value_or(NO_VALUE);
	ASSERT_NE(NO_VALUE, value);
	ASSERT_EQ(ELEMENT_1, value);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
