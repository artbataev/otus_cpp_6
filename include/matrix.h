#pragma once
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
