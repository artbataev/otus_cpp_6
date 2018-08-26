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

    std::tuple<int, T> get_element_by_id(int elem_num) const {
        int i = 0;
        for(const auto& [key, value]:elements) {
            if (value == DefaultValue) continue;
            if (i == elem_num) {
                return std::make_tuple(key, value);
            } else {
                i++;
            }
        }
        return std::make_tuple(-1, DefaultValue);
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

    // very slow! works only for 2-d matrix
    std::tuple<int, int, T> get_element_by_id(int elem_num) {
        int i = 0;
        for(const auto& [key, value]:elements) {
            size_t cur_size = value.size();
            if (i + cur_size > elem_num) {
                return std::tuple_cat(std::make_tuple(key), value.get_element_by_id(elem_num - i));
            } else {
                i += cur_size;
            }
        }
        return std::make_tuple(-1, -1, DefaultValue);
    }

    class MatrixIterator
    {
    public:
        using value_type = Matrix;
        using reference = std::tuple<int, int, T>;
        using pointer = Matrix* ;
        using difference_type = int;
        MatrixIterator(Matrix& parent, int elem_num_) : parent_(parent), elem_num(elem_num_) { }
        MatrixIterator operator++() { MatrixIterator i = *this; elem_num++; return i; }
        reference operator*() { return parent_.get_element_by_id(elem_num); }
        bool operator==(const MatrixIterator& rhs) { return elem_num == rhs.elem_num; }
        bool operator!=(const MatrixIterator& rhs) { return elem_num != rhs.elem_num; }
    private:
        int elem_num;
        Matrix& parent_;
    };

    MatrixIterator begin()
    {
        return MatrixIterator(*this, 0);
    }

    MatrixIterator end()
    {
        return MatrixIterator(*this, size());
    }

private:
    std::map<int, Matrix<T, DefaultValue, NumDimensionsRed>> elements;

};

int main() {
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    assert(matrix.size() == 0); // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

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

//    Matrix<int, -1, 2> matrix2;
//    matrix2 = matrix;
    return 0;
}
