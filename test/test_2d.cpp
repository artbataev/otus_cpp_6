#include <matrix.h>

#include "gtest/gtest.h"

TEST(SomeTest, SomeTest) {
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    ASSERT_EQ(matrix.size(), 0); // все ячейки свободны
    auto a = matrix[0][0];
    ASSERT_EQ(a, -1);
    EXPECT_EQ(matrix.size(), 0);
    matrix[100][100] = 314;
    ASSERT_EQ(matrix[100][100], 314);
    EXPECT_EQ(matrix.size(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
