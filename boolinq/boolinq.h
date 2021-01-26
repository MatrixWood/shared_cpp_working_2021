#pragma once

#include <limits.h>

#include <functional>
#include <tuple>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>

struct LinqEndException {};

enum BytesDirection {
    BytesFirstToLast,
    BytesLastToFirst,
};

enum BitsDirection {
    BitsHighToLow,
    BitsLowToHigh,
};

template <typename S, typename T>
class Linq {
    std::function<T(S &)> nextFunc;
    S storage;

public:
    typedef T value_type;

    Linq() : nextFunc(), storage() {}

    Linq(S storage, std::function<T(S &)> nextFunc)
        : nextFunc(nextFunc), storage(storage) {}

    T next() {
        return nextFunc(storage);
    }
};

template <typename T>
Linq<std::pair<T, T>, typename std::iterator_traits<T>::value_type>
from(const T& begin, const T& end) {
    return Linq<std::pair<T, T>, typename std::iterator_traits<T>::value_type>(
        std::make_pair(begin, end),
        [](std::pair<T, T>& pair) {
            if (pair.first == pair.second) throw LinqEndException();
            return *(pair.first++);
        }
    );
}

template <template <class, class> class TV, typename TT, typename TU>
auto from(const TV<TT, TU>& container)
    -> decltype(from(container.cbegin(), container.cend())) {
    return from(container.cbegin(), container.cend());
}