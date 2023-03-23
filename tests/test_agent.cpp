#include <gtest/gtest.h>
#include "agent.h"

// Define your test cases here
TEST(MyTest, Example) {
    // Write your test code here
    ASSERT_EQ(2 + 2, 4);
}

// Define your main function to run all tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
