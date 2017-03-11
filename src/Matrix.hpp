#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <array>
#include <algorithm>
#include "view.hpp"
#include "utils.hpp"

template<bool IsMatrixView = false>
struct MatrixViewProxy {
    template<class T>
    static inline T& get(T& t) { return t; }
};

template<>
struct MatrixViewProxy<true> {
    template<class T>
    static inline T& get(T* t) { return *t; }
};


template<int N, int M, class T = double, bool IsMatrixView = false>
class Matrix {
    using MatrixDataBase = std::array<T, N*M>;
    using MatrixDataView = std::array<T*, N*M>;
    using MatrixData = typename std::conditional<IsMatrixView, MatrixDataView, MatrixDataBase>::type;
    MatrixData _data;

public:
    static constexpr int height = N;
    static constexpr int width = M;
    using type = T;

    Matrix() {}

    template<class... E>
    Matrix(E&&... e) : _data{std::forward<E>(e)...} {}

    template<int N2, int M2, bool AnyView>
    explicit Matrix(Matrix<N2, M2, T, AnyView>& other, int x, int y) {
        for (int ly = 0; ly < N; ++ly) {
            for (int lx = 0; lx < M; ++lx) {
                _data[ly * M + lx] = &other.data(ly+y, lx+x);
            }
        }
    }

    Matrix(Matrix&& other) : _data{std::move(other._data)} {}
    Matrix(Matrix& other) : _data{other._data} {}
    Matrix(const Matrix& other) : _data{other._data} {}

    constexpr void size() const { return N * M; }

    inline MatrixData data() { return MatrixViewProxy<IsMatrixView>::template get(_data); };
    inline decltype(_data) data() const { return MatrixViewProxy<IsMatrixView>::template get(_data); }
    inline T& data(int i) { return MatrixViewProxy<IsMatrixView>::template get(_data[i]); }
    inline T data(int i) const { return MatrixViewProxy<IsMatrixView>::template get(_data[i]); }
    inline T& data(int y, int x) { return MatrixViewProxy<IsMatrixView>::template get(_data[y*M+x]); }
    inline T data(int y, int x) const { return MatrixViewProxy<IsMatrixView>::template get(_data[y*M+x]); }

    template<class Fn>
    void forEachCoord(Fn f) const; ///< (int y, int x)

    void swapRows(int y1, int y2);
    bool reduce();

    Matrix& operator+=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
};

template<int N, int M, class T, bool IsMatrixView>
void Matrix<N, M, T, IsMatrixView>::swapRows(int y1, int y2) {
    modify2(row_view(y1, *this), row_view(y2, *this), [](T& left, T& right) {
        std::swap(left, right);
    });
}

template<int N, int M, class T, bool IsMatrixView>
bool Matrix<N, M, T, IsMatrixView>::reduce() {
    auto view = column_view(0, *this);
    auto viewEnd = view.end();
    auto it = std::find_if(view.begin(), viewEnd, [](T n) { return n != 0; });
    if (it == viewEnd) return false;

    int rowIndex = std::distance(view.begin(), it);
    if (rowIndex != 0) swapRows(0, rowIndex);

    return true;
}

template<int N, int M, class T, bool IsMatrixView>
Matrix<N, M, T, IsMatrixView>& Matrix<N, M, T, IsMatrixView>::operator+=(const Matrix<N, M, T, IsMatrixView>& other) {
    auto intoIt = _data.begin();
    std::for_each(other._data.begin(), other._data.end(), [&intoIt](const T& fromValue){
            *intoIt++ += fromValue;
        });
    return *this;
}

template<int N, int M, class T, bool IsMatrixView>
Matrix<N, M, T, IsMatrixView>& Matrix<N, M, T, IsMatrixView>::operator*=(const Matrix<N, M, T, IsMatrixView>& other) {
    auto temp = *this * other;
    _data = std::move(temp._data);
}

template<int N, int M, class T, bool IsMatrixView>
std::ostream& operator<<(std::ostream& os, const Matrix<N, M, T, IsMatrixView>& m) {
    m.forEachCoord([&](int y, int x){
        os << std::setprecision(3) << std::setw(5) << m.data(y, x) << " ";
        if (x == N-1) os << std::endl;
    });
    return os;
}

template<int N, int M, class T, bool IsMatrixView>
Matrix<N, M, T, IsMatrixView> operator+(Matrix<N, M, T, IsMatrixView> lhs, const Matrix<N, M, T, IsMatrixView>& rhs) {
    lhs += rhs;
    return lhs;
}

template<int N, int M, class T, bool IsMatrixView>
template<class Fn>
void Matrix<N, M, T, IsMatrixView>::forEachCoord(Fn f) const {
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
