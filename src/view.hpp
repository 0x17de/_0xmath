#ifndef MATRIX_VIEW_H
#define MATRIX_VIEW_H

#include <iterator>
#include <type_traits>


template<int N, int M, class T>
class Matrix;


class MatrixRowView;
class MatrixColumnView;


template<class Type>
struct matrix_iterator_distance {
    template<class M>
    static constexpr int value();
};
template<>
template<class M>
constexpr int matrix_iterator_distance<MatrixColumnView>::value() { return std::decay<M>::type::width; }
template<>
template<class M>
constexpr int matrix_iterator_distance<MatrixRowView>::value() { return 1; }


template<class Type>
struct matrix_iterator_distance_to_start {
    template<class M>
    static constexpr int value(int i);
};
template<>
template<class M>
constexpr int matrix_iterator_distance_to_start<MatrixColumnView>::value(int i) { return i; }
template<>
template<class M>
constexpr int matrix_iterator_distance_to_start<MatrixRowView>::value(int i) { return i*std::decay<M>::type::width; }


template<class Type>
struct matrix_iterator_distance_to_end {
    template<class M>
    static constexpr int value(int i);
};
template<>
template<class M>
constexpr int matrix_iterator_distance_to_end<MatrixColumnView>::value(int i) { return i+std::decay<M>::type::width * std::decay<M>::type::height; }
template<>
template<class M>
constexpr int matrix_iterator_distance_to_end<MatrixRowView>::value(int i) { return (i+1)*std::decay<M>::type::width; }


template<class Type, class M>
class matrix_iterator {
    using _M = typename std::decay<M>::type;

    mutable int _index;
    M& _matrix;

public:
    matrix_iterator() { }
    matrix_iterator(int index, M& matrix) : _index(index), _matrix(matrix) { }

    matrix_iterator(typename std::remove_const<matrix_iterator>::type& other) : _index(other._index), _matrix(other._matrix) { }
    matrix_iterator(const matrix_iterator& other) : _index(other._index), _matrix(other._matrix) { }

    inline typename _M::type operator*() { return _matrix.data(_index); }
    inline typename _M::type operator*() const { return _matrix.data(_index); }
    inline matrix_iterator& operator++() { _index += matrix_iterator_distance<Type>::template value<M>(); return *this; }
    inline matrix_iterator operator++(int) { auto tmp = *this; _index += matrix_iterator_distance<Type>::template value<M>(); return this; }
    inline bool operator==(const matrix_iterator& other) const { return _index == other._index; }
    inline bool operator!=(const matrix_iterator& other) const { return _index != other._index; }
};


namespace std {
    template<class Type, class M>
    struct iterator_traits<matrix_iterator<Type, M>> {
        using value_type = typename M::type;
        using difference_type = int;
        using iterator_category = std::forward_iterator_tag;
    };
}


template<class Type, class M>
class matrix_view {
    int _index;
    M& _matrix;

public:
    matrix_view(int index, M& matrix) : _index(index), _matrix(matrix) { }

    constexpr matrix_iterator<Type, M> begin() { return matrix_iterator<Type, M>(matrix_iterator_distance_to_start<Type>::template value<M>(_index), _matrix); }
    constexpr matrix_iterator<Type, M> end() { return matrix_iterator<Type, M>(matrix_iterator_distance_to_end<Type>::template value<M>(_index), _matrix); }
};

template<class M>
inline matrix_view<MatrixColumnView, M> column_view(int index, M&& m) { return matrix_view<MatrixColumnView, M>(index, std::forward<M>(m)); }
template<class M>
inline matrix_view<MatrixRowView, M> row_view(int index, M&& m) { return matrix_view<MatrixRowView, M>(index, std::forward<M>(m)); }

#endif
