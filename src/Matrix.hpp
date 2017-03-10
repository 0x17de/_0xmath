#include <iostream>
#include <iomanip>
#include <array>
#include <algorithm>
#include <type_traits>


template<class R, class... T>
struct first_of {
    using type = R;
};
template<class R, class... T>
using first_of_t = typename first_of<R, T...>::type;

template<size_t N, size_t M, class T>
class Matrix;

template<size_t N, size_t M, class T>
class Matrix {
    std::array<T, N*M> _data;

public:
    Matrix() {}

    template<class... E>
    Matrix(E&&... e) : _data{std::forward<E>(e)...} {}

    Matrix(Matrix&& other) : _data{std::move(other._data)} {}
    Matrix(Matrix& other) : _data{other._data} {}
    Matrix(const Matrix& other) : _data{other._data} {}

    Matrix& operator+=(const Matrix& other) {
        auto fromIt = other._data.begin();
        auto intoIt = _data.begin();
        std::for_each(other._data.begin(), other._data.end(), [&intoIt](const T& fromValue){
                *intoIt++ += fromValue;
            });
    }
    Matrix& operator*=(const Matrix& other) {
        auto temp = *this * other;
        _data = std::move(temp._data);
    }

    inline decltype((_data)) data() { return _data; }
    inline decltype((_data)) data() const { return _data; }
    inline T& data(size_t y, size_t x) { return _data[y*M+x]; }
    inline T data(size_t y, size_t x) const { return _data[y*M+x]; }
};

template<size_t N, size_t M, class T>
std::ostream& operator<<(std::ostream& os, const Matrix<N, M, T>& m) {
    for (size_t y = 0; y < N; ++y) {
        for (size_t x = 0; x < M; ++x) {
            os << std::setprecision(3) << std::setw(5) << m.data(y, x) << " ";
        }
        os << std::endl;
    }
    return os;
}

template<size_t N, size_t M, class T>
Matrix<N, M, T> operator+(Matrix<N, M, T> lhs, const Matrix<N, M, T>& rhs) {
    lhs += rhs;
    return lhs;
}

template<size_t N1, size_t M1, size_t M2, class T>
Matrix<N1, M2, T> operator*(const Matrix<N1, M1, T>& first, const Matrix<M1, M2, T>& other) {
    Matrix<N1, M2, T> result;

    for (size_t y = 0; y < N1; ++y) {
        for (size_t x = 0; x < M2; ++x) {
            size_t n = 0;
            for (size_t i = 0; i < M1; ++i) {
                n += first.data(y, i) * other.data(i, x);
            }
            result.data(y, x) = n;
        }
    }
    return result;
}
