#include <iostream>
#include <cassert>
#include <vector>
#include <map>

template<typename T,
        int DefaultValue,
        int NumDimensions = 2>
class Matrix;


template<typename T, int DefaultValue>
class Matrix<T, DefaultValue, 1> {
public:
    Matrix() = default;

    T& operator[](const int idx) {
        if (elements.count(idx) == 0) elements[idx] = DefaultValue;
        return elements.at(idx);
    }

    size_t size() const {
        size_t result = 0;
        for(const auto& [key, value]:elements) {
            if (value != DefaultValue) {
                result++;
            }
        }
        return result;
    }

    Matrix& operator=(const T& rhs) {
        if(this == &rhs)
            return *this;
        elements = rhs.elements;
        return *this;
    }

private:
    std::map<int, T> elements;
};

template<typename T,
        int DefaultValue,
        int NumDimensions>
class Matrix {
public:
    Matrix() = default;
    constexpr static int NumDimensionsRed = NumDimensions - 1;

    size_t size() const {
        size_t result = 0;
        for(const auto& [key, value]:elements) {
            result += value.size();
        }
        return result;
    }

    Matrix<T, DefaultValue, NumDimensionsRed>& operator[](const int idx) {
        return elements[idx];
    }

    Matrix& operator=(const T& rhs) {
        if(this == &rhs)
            return *this;
        elements = rhs.elements;
        return *this;
    }

//    std::tuple<int, int, T> *begin() {
//
//    }
//
//    std::tuple<int, int, T> *end() {
//
//    }

private:
    std::map<int, Matrix<T, DefaultValue, NumDimensionsRed>> elements;

};

int main() {
    Matrix<int, -1, 2> matrix; // бесконечная матрица int заполнена значениями -1
    assert(matrix.size() == 0); // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

//    // выведется одна строка
//    // 100100314
//    for(auto c: matrix)
//    {
//        int x;
//        int y;
//        int v;
//        std::tie(x, y, v) = c;
//        std::cout << x << y << v << std::endl;
//    }

//    Matrix<int, -1, 2> matrix2;
//    matrix2 = matrix;
    return 0;
}
