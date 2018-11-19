#include <matrix.h>

#include "gtest/gtest.h"

TEST(Test2d, TestStatic) {
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    ASSERT_EQ(matrix.size(), 0); // все ячейки свободны
    auto a = matrix[0][0];
    ASSERT_EQ(a, -1);
    EXPECT_EQ(matrix.size(), 0);
    matrix[100][100] = 314;
    ASSERT_EQ(matrix[100][100], 314);
    EXPECT_EQ(matrix.size(), 1);
}

TEST(Test2d, TestIteration) {
    Matrix<int, -1> matrix;
    matrix[100][100] = 314;
    testing::internal::CaptureStdout();
    for (auto c: matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "100100314\n");
}

TEST(Test2d, TestAssignmentOperator) {
    Matrix<int, -1> matrix;
    ((matrix[100][100] = 314) = 0) = 217;
    ASSERT_EQ(matrix[100][100], 217);
    ASSERT_EQ(matrix.size(), 1);
}

TEST(Test3d, TestStatic) {
    Matrix<int, 2, 3> matrix3d;
    auto a = matrix3d[0][0][0];
    ASSERT_EQ(a, 2); // default - 2
    ASSERT_EQ(matrix3d.size(), 0);
    matrix3d[99][99][99] = 314;
    ASSERT_EQ(matrix3d.size(), 1);
    matrix3d[1][2][3] = 2; // default - doesn't change matrix size
    ASSERT_EQ(matrix3d.size(), 1);
}

TEST(Test3d, TestIteration) {
    Matrix<int, -1, 3> matrix3d;
    matrix3d[1][2][4] = 314;
    matrix3d[3][6][8] = 111;
    matrix3d[128][256][512] = 1024;

    testing::internal::CaptureStdout();
    for (auto c: matrix3d) {
        int x;
        int y;
        int z;
        int v;
        std::tie(x, y, z, v) = c;
        std::cout << x << " " << y << " " << z << " " << v << std::endl;
    }
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "1 2 4 314\n3 6 8 111\n128 256 512 1024\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
