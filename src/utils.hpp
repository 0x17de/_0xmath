#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

template<class T, class IT1, class IT2, class Fn>
inline T zipReduce(const IT1& leftIterable, const IT2& rightIterable, T initial, Fn f) {
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
