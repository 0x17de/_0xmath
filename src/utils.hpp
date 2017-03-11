#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

template<class IT1, class IT2, class Fn>
inline void modify2(IT1 leftIterable, IT2 rightIterable, Fn f) {
    auto leftIt = leftIterable.begin();
    auto rightIt = rightIterable.begin();
    auto leftEnd = leftIterable.end();

    while (leftIt != leftEnd) {
        f(*leftIt, *rightIt);
        ++leftIt; ++rightIt;
    }
}

template<class T, class IT1, class IT2, class Fn>
inline T zipReduce(IT1 leftIterable, IT2 rightIterable, T initial, Fn f) {
    auto leftIt = leftIterable.begin();
    auto rightIt = rightIterable.begin();
    auto leftEnd = leftIterable.end();

    while (leftIt != leftEnd) {
        initial = f(*leftIt, *rightIt, initial);
        ++leftIt; ++rightIt;
    }
    return initial;
}

#endif
