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

    void for_each_i(std::function<void(T)> apply) const {
        Linq<S, T> linq = *this;
        try {
            while (true) {
                apply(linq.next());
            }
        } catch (LinqEndException &) {}
    }

    void for_each(std::function<void(T)> apply) const {
        return for_each_i([apply](T value) { return apply(value); });
    }

    Linq<std::tuple<Linq<S, T>, int>, T> where_i(std::function<bool(T, int)> filter) const {
        return Linq<std::tuple<Linq<S, T>, int>, T>(
            std::make_tuple(*this, 0),
            [filter](std::tuple<Linq<S, T>, int>& tuple) {
                Linq<S, T>& linq = std::get<0>(tuple);
                int& index = std::get<1>(tuple);

                while (true) {
                    T ret = linq.next();
                    if (filter(ret, index++))
                        return ret;
                }
            }
        );
    }

    Linq<std::tuple<Linq<S, T>, int>, T> where(std::function<bool(T)> filter) const {
        return where_i([filter](T value, int) { return filter(value); });
    }

    bool any(std::function<bool(T)> predicate) const {
        Linq<S, T> linq = *this;
        try {
            while (true) {
                if (predicate(linq.next()))
                    return true;
            }
        } catch (LinqEndException &) {}
        return false;
    }

    bool any() const {
        return any([](T value) { return static_cast<bool>(value); });
    }

    bool all(std::function<bool(T)> predicate) const {
        return !any([predicate](T value) { return !predicate(value); });
    }

    bool all() const {
        return all([](T value) {return static_cast<bool>(value); });
    }

    bool contains(const T& item) const {
        return any([&item](T value) { return value == item; });
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

template <typename T>
Linq<std::pair<T, T>, typename std::iterator_traits<T>::value_type>
from(const T& it, int n) {
    return from(it, it + n);
}

template <typename T, int N>
Linq<std::pair<const T*, const T*>, T>
from(T (&array)[N]) {
    return from((const T*)(&array), (const T*)(&array) + N);
}

template <template <class, class> class TV, typename TT, typename TU>
auto from(const TV<TT, TU>& container)
    -> decltype(from(container.cbegin(), container.cend())) {
    return from(container.cbegin(), container.cend());
}