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
        throw std::logic_error("no element");
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

    // very slow!
    decltype(auto) get_element_by_id(int elem_num) const {
        int i = 0;
        for(const auto& [key, value]:elements) {
            size_t cur_size = value.size();
            if (i + cur_size > elem_num) {
                return std::tuple_cat(std::make_tuple(key), value.get_element_by_id(elem_num - i));
            } else {
                i += cur_size;
            }
        }
        throw std::logic_error("no element");
    }

    class MatrixIterator
    {
    public:
        using value_type = Matrix;
        using pointer = Matrix* ;
        using difference_type = int;
        MatrixIterator(Matrix& parent, int elem_num_) : parent_(parent), elem_num(elem_num_) { }
        MatrixIterator operator++() { MatrixIterator i = *this; elem_num++; return i; }
        decltype(auto) operator*() { return parent_.get_element_by_id(elem_num); }
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
