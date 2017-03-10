#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <array>
#include <algorithm>
#include "view.hpp"
#include "utils.hpp"


template<int N, int M, class T>
class Matrix {
    std::array<T, N*M> _data;

public:
    static constexpr int height = N;
    static constexpr int width = M;
    using type = T;

    Matrix() {}

    template<class... E>
    Matrix(E&&... e) : _data{std::forward<E>(e)...} {}

    Matrix(Matrix&& other) : _data{std::move(other._data)} {}
    Matrix(Matrix& other) : _data{other._data} {}
    Matrix(const Matrix& other) : _data{other._data} {}

    constexpr void size() const { return N * M; }

    inline decltype((_data)) data() { return _data; }
    inline decltype(_data) data() const { return _data; }
    inline T& data(int i) { return _data[i]; }
    inline T data(int i) const { return _data[i]; }
    inline T& data(int y, int x) { return _data[y*M+x]; }
    inline T data(int y, int x) const { return _data[y*M+x]; }

    template<class Fn>
    void forEachCoord(Fn f); ///< (int y, int x)

    void swapRows(int y1, int y2);
    bool reduce();

    Matrix& operator+=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
};


template<int N, int M, class T>
bool Matrix<N, M, T>::reduce() {
    /*
    auto view = column_view(0, *this);
    auto viewEnd = view.end();
    auto it = std::find_if(view.begin(), viewEnd, [](T n) { return n != 0; });
    if (it == viewEnd) return false;

    T firstNumber = *it;
    std::cout << firstNumber << std::endl;
    if (firstNumber != 1) {

    }
    */

    //swapRows(y, 0);
    return true;
}

template<int N, int M, class T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& other) {
    auto intoIt = _data.begin();
    std::for_each(other._data.begin(), other._data.end(), [&intoIt](const T& fromValue){
            *intoIt++ += fromValue;
        });
    return *this;
}

template<int N, int M, class T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const Matrix<N, M, T>& other) {
    auto temp = *this * other;
    _data = std::move(temp._data);
}

template<int N, int M, class T>
std::ostream& operator<<(std::ostream& os, const Matrix<N, M, T>& m) {
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            os << std::setprecision(3) << std::setw(5) << m.data(y, x) << " ";
        }
        os << std::endl;
    }
    return os;
}

template<int N, int M, class T>
Matrix<N, M, T> operator+(Matrix<N, M, T> lhs, const Matrix<N, M, T>& rhs) {
    lhs += rhs;
    return lhs;
}

template<int N, int M, class T>
template<class Fn>
void Matrix<N, M, T>::forEachCoord(Fn f) {
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            f(y, x);
        }
    }
}


template<int N1, int M1, int M2, class T>
Matrix<N1, M2, T> operator*(const Matrix<N1, M1, T>& first, const Matrix<M1, M2, T>& other) {
    Matrix<N1, M2, T> result;
    result.forEachCoord([&](int y, int x){
        result.data(y, x) = zipReduce(row_view(y, first), column_view(x, other), 0,
                                      [](T left, T right, T prev){
                                          return left * right + prev;
                                      });
    });
    return result;
}

#endif
