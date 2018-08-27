#include <iostream>
#include <cassert>
#include <matrix.h>


int main() {
    Matrix<int, 0> main_matrix;
    const int n = 10;
    // fill main diagonal
    for(int i = 0; i < n; i++) {
        main_matrix[i][i] = i;
    }

    // fill counter diagonal
    for(int i = n - 1; i >= 0; i--) {
        main_matrix[i][n - i - 1] = i;
    }
    std::cout << "Matrix:" << std::endl;
    for(int i = 1; i < n-1; i++) {
        for(int j = 1; j < n-1; j++) {
            if(j > 1) std::cout << " ";
            std::cout << main_matrix[i][j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Matrix size: " << main_matrix.size() << std::endl;

    std::cout << std::endl;
    std::cout << "Matrix elements:" << std::endl;
    for(auto elem: main_matrix)
    {
        int i, j, value;
        std::tie(i, j, value) = elem;
        std::cout << i << " " << j << "\t" << value << std::endl;
    }

    std::cout << std::endl << std::endl;
    std::cout << "2d matrix tests" << std::endl;
//    moved to tests
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
//    assert(matrix.size() == 0); // все ячейки свободны
//    auto a = matrix[0][0];
//    assert(a == -1);
//    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
//    assert(matrix.size() == 1);

    // выведется одна строка
    // 100100314
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    // 3d matrix test
    std::cout << std::endl;
    std::cout << "3d matrix tests" << std::endl;
    Matrix<int, -1, 3> matrix3d;
    auto q = matrix3d[0][0][0];
    assert(matrix3d.size() == 0);
    matrix3d[99][99][99] = 314;
    assert(matrix3d.size() == 1);

    for(auto c: matrix3d)
    {
        int x;
        int y;
        int z;
        int v;
        std::tie(x, y, z, v) = c;
        std::cout << x << y << z << v << std::endl;
    }
    return 0;
}
