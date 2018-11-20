#pragma once

#include <map>
#include <iostream>

template<typename T,
        int DefaultValue,
        int NumDimensions = 2>
class Matrix;

template<typename T, int DefaultValue>
class Matrix<T, DefaultValue, 1> {
public:
    Matrix() = default;

    class MatrixElement {
    public:
        MatrixElement(T data_, int id_, Matrix *matrix_ptr_) :
                data{data_}, idx{id_}, matrix_pointer(matrix_ptr_) {}

        operator T() const {
            return data;
        }

        MatrixElement& operator=(const T& rhs) {
            data = rhs;

            if (data != DefaultValue)
                matrix_pointer->move_to_known(idx);
            else
                matrix_pointer->move_to_unknown(idx);

            return *this;
        }

        T item() const {
            return data;
        }

        bool operator==(const T rhs) const {
            return data == rhs;
        }

        bool operator!=(const T rhs) const {
            return data != rhs;
        }

        bool operator==(const MatrixElement rhs) const {
            return data == rhs.data;
        }

        bool operator!=(const MatrixElement rhs) const {
            return data != rhs.data;
        }

    private:
        T data;
        Matrix *matrix_pointer;
        int idx;
    };


    MatrixElement& operator[](const int idx) {
        if (elements.count(idx) == 0) {
            if (elements_temp.count(idx) > 0)
                return *elements_temp.at(idx);
            auto elem = std::make_shared<MatrixElement>(DefaultValue, idx, this);
            elements_temp.insert(std::make_pair(idx, elem));
            return *elem;
        }
        return *elements.at(idx);
    }

    size_t size() const {
        size_t result = 0;
        for (const auto&[key, value]:elements) {
            result++;
        }
        return result;
    }

    Matrix& operator=(const T& rhs) {
        if (this == &rhs)
            return *this;
        elements = rhs.elements;
        return *this;
    }

    std::tuple<int, T> get_element_by_id(int elem_num) const {
        int i = 0;
        for (const auto&[key, value]:elements) {
            if (i == elem_num) {
                return std::make_tuple(key, T(*value));
            } else {
                i++;
            }
        }
        throw std::logic_error("no element");
    }

private:
    std::map<int, std::shared_ptr<MatrixElement>> elements;
    std::map<int, std::shared_ptr<MatrixElement>> elements_temp;

    void move_to_known(int idx) {
        if (elements_temp.count(idx) > 0) {
            elements.insert(std::make_pair(idx, elements_temp.at(idx)));
            elements_temp.erase(idx);
        }
    }

    void move_to_unknown(int idx) {
        if (elements.count(idx) > 0) {
            elements.erase(idx);
        }
    }
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
        for (const auto&[key, value]:elements) {
            result += value.size();
        }
        return result;
    }

    Matrix<T, DefaultValue, NumDimensionsRed>& operator[](const int idx) {
        return elements[idx];
    }

    Matrix& operator=(const T& rhs) {
        if (this == &rhs)
            return *this;
        elements = rhs.elements;
        return *this;
    }

    decltype(auto) get_element_by_id(int elem_num) const {
        int i = 0;
        for (const auto&[key, value]:elements) {
            size_t cur_size = value.size();
            if (i + cur_size > elem_num) {
                return std::tuple_cat(std::make_tuple(key), value.get_element_by_id(elem_num - i));
            } else {
                i += cur_size;
            }
        }
        throw std::logic_error("no element");
    }

    class iterator {
    public:
        using value_type = Matrix;
        using pointer = Matrix *;
        using difference_type = int;

        iterator(Matrix& parent, int elem_num_) : parent_(parent), elem_num(elem_num_) {}

        iterator operator++() {
            iterator i = *this;
            elem_num++;
            return i;
        }

        decltype(auto) operator*() { return parent_.get_element_by_id(elem_num); }

        bool operator==(const iterator& rhs) { return elem_num == rhs.elem_num; }

        bool operator!=(const iterator& rhs) { return elem_num != rhs.elem_num; }

    private:
        int elem_num;
        Matrix& parent_;
    };

    iterator begin() {
        return iterator(*this, 0);
    }

    iterator end() {
        return iterator(*this, size());
    }

    iterator cbegin() const {
        return iterator(*this, 0);
    }

    iterator cend() const {
        return iterator(*this, size());
    }

private:
    std::map<int, Matrix<T, DefaultValue, NumDimensionsRed>> elements;
};
