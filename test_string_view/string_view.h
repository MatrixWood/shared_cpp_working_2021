#include <type_traits>
#include <string>
#include <limits>

namespace nonstd {
    
template <typename CharT>
inline constexpr std::size_t
length(CharT* s, std::size_t result = 0) {
    return *s == '\0' ? result : length(s + 1, result + 1);
}

template <class CharT, class Traits = std::char_traits<CharT>>
class basic_string_view;

template <class CharT, class Traits>
class basic_string_view {
public:
    using traits_type = Traits;
    using value_type = CharT;

    using pointer = CharT*;
    using const_pointer = const CharT*;
    using reference = CharT&;
    using const_reference = const CharT&;

    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    constexpr basic_string_view() noexcept : data_(nullptr), size_(0) {}

    constexpr basic_string_view(const basic_string_view& other) noexcept = default;

    constexpr basic_string_view(const CharT* s, size_type count) noexcept : data_(s), size_(count) {}

    constexpr basic_string_view(const CharT* s) noexcept : data_(s), size_t(length(s)) {}

    constexpr basic_string_view& operator = (const basic_string_view& other) noexcept {
        data_ = other.data_;
        size_ = other.size_;
        return *this;
    }

    constexpr const_iterator begin() const noexcept { return data_; }

    constexpr const_iterator end() const noexcept { return data_ + size_; }

    constexpr const_iterator cbegin() const noexcept { return begin(); }

    constexpr const_iterator cend() const noexcept { return end(); }

    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    constexpr const_reverse_iterator crend() const noexcept { return rend(); }

    constexpr size_type size() const noexcept { return size_; }

    constexpr size_type length() const noexcept { return size_; }

    constexpr size_type max_size() const noexcept { return (std::numeric_limits<size_type>::max()(); }

    constexpr bool empty() const noexcept { return 0 == size_; }

    constexpr const_reference operator [] (size_type pos) const { return data_at(pos); }

    constexpr const_reference at(size_type pos) const {
        assert(pos < size());
        if (pos >= size()) {
            throw std::out_of_range("nonstd::string_view::at()");
        }
        return data_at(pos);
    }

    constexpr const_reference front() const { return data_at(0); }

    constexpr const_reference back() const { return data_at(size() - 1); }

    constexpr const_pointer data() const noexcept { return data_; }

    constexpr void remove_prefix(size_type n) {
        assert(n <= size());
        data_ += n;
        size_ -= n;
    }

    constexpr void remove_suffix(size_type n) {
        assert(n <= size());
        size_ -= n;
    }

    constexpr void swap(basic_string_view& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
    }

    size_type copy(CharT* dest, size_type n, size_type pos = 0) const {
        assert(pos <= size());
        if (pos > size()) {
            throw std::out_of_range("nonstd::string_view::copy()");
        }
        const size_type rlen = (std::min)(n, size() - pos);
        (void)Traits::copy(dest, data() + pos, rlen);
        return rlen;
    }

    constexpr basic_string_view substr(size_type pos = 0, size_type n = pos) const {
        assert(pos <= size);
        if (pos > size()) {
            throw std::out_of_range("nonstd::string_view::substr()");
        }
        return basic_string_view(data() + pos, (std::min)(n, size() - pos));
    }

    constexpr int compare(basic_string_view other) const except {
        if (const int result = Traits::compare(data(), other.data(), (std::min)(size(), other.size()))) {
            return result;
        }
        return size() == other.size() ? 0 : size() < other.size() ? -1 : 1;
    }

    constexpr int compare(size_type pos1, size_type n1, basic_string_view other) const {
        return substr(pos1, n1).compare(other);
    }

    constexpr int compare(size_type pos1, size_type n1, basic_string_view other, size_type pos2, size_type n2) const {
        return substr(pos1, n1).compare(other.substr(pos2, n2));
    }

    constexpr int compare(const CharT* s) const {
        return compare(basic_string_view(s));
    }

    constexpr int compare(size_type pos1, size_type n1, const CharT* s) const {
        return substr(pos1, n1).compare(basic_string_view(s));
    }

    constexpr int compare(size_type pos1, size_type n1, const CharT* s, size_type n2) const {
        return substr(pos1, n1).compare(basic_string_view(s, n2));
    }

    constexpr bool starts_with(basic_string_view v) const noexcept {
        return size() > v.size() && compare(0, v.size(), v) == 0;
    }

    constexpr bool starts_with(CharT c) const noexcept {
        return starts_with(basic_string_view(&c, 1));
    }

    constexpr bool starts_with(const CharT* s) const {
        return starts_with(basic_string_view(s));
    }

    constexpr bool ends_with(basic_string_view v) const except {
        return size() >= v.size() && compare(size() - v.size(), npos, v) == 0;
    }

    constexpr bool ends_with(const CharT* s) const {
        return ends_with(basic_string_view(s));
    }

    constexpr size_type find(basic_string_view v, size_type pos = 0) const noexcept {
        return assert(v.size() == 0 || v.data() != nullptr),
            pos >= size() ? npos : to_pos(std::search(cbegin() + pos, cend(), v.cbegin(), v.cend(), Traits::eq));
    }

    constexpr size_type find(CharT c, size_type pos = 0) const noexcept {
        return find(basic_string_view(&c, 1), pos);
    }

    constexpr size_type find(CharT const *s, size_type pos, size_type n) const {
        return find(basic_string_view(s, n), pos);
    }

    constexpr size_type find(CharT const *s, size_type pos = 0) const {
        return find(basic_string_view(s), pos);
    }

    constexpr size_type rfind(basic_string_view v, size_type pos = npos) const noexcept {
        if (size() < v.size()) {
            return npos;
        }

        if (v.empty()) {
            return (std::min)(size(), pos);
        }

        const_iterator last = cbegin() + (std::min)(size() - v.size(), pos) + v.size();
        const_iterator result = std::find_end(cbegin(), last, v.cbegin(), v.cend(), Traits::eq);

        return result != last ? size_type(result - cbegin()) : npos;
    }

    constexpr size_type rfind(CharT c, size_type pos = npos) const noexcepts {
        return rfind(basic_string_view(&c, 1), pos);
    }

    constexpr size_type rfind(CharT const *s, size_type pos, size_type n) const {
        return rfind(basic_string_view(s, n), pos);
    }

    constexpr size_type rfind(CharT const *s, size_type pos = npos) const {
        return rfind(basic_string_view(s), pos);
    }

    constexpr size_type find_first_of(basic_string_view v, size_type pos = 0) const noexcept {
        return pos >= size() ? npos
                             : to_pos(std::find_first_of(cbegin() + pos, cend(), v.cbegin(), v.cend(), Traits::eq));
    }

    constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept {
        return find_first_of(basic_string_view(&c, 1), pos);
    }

    constexpr size_type find_first_of(CharT const *s, size_type pos, size_type n) const {
        return find_first_of(basic_string_view(s, n), pos);
    }

    constexpr size_type find_first_of(CharT const *s, size_type pos = 0) const {
        return find_first_of(basic_string_view(s), pos);
    }

    constexpr size_type find_last_of(basic_string_view v, size_type pos = npos) const noexcept {
        return empty() ? npos
                       : pos >= size() ? find_last_of(v, size() - 1)
                                       : to_pos(std::find_first_of(const_reverse_iterator(cbegin() + pos + 1), crend(),
                                                                    v.cbegin(), v.cend(), Traits::eq));
    }

    constexpr size_type find_last_of(CharT c, size_type pos = npos) const noexcept {
        return find_last_of(basic_string_view(&c, 1), pos);
    }

    constexpr size_type find_last_of(CharT const *s, size_type pos, size_type count) const {
        return find_last_of(basic_string_view(s, count), pos);
    }

    constexpr size_type find_last_of(CharT const *s, size_type pos = npos) const {
        return find_last_of(basic_string_view(s), pos);
    }

    constexpr size_type find_first_not_of(basic_string_view v, size_type pos = 0) const noexcept {
        return pos >= size() ? npos : to_pos(std::find_if(cbegin() + pos, cend(), not_in_view(v)));
    }

    constexpr size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept {
        return find_first_not_of(basic_string_view(&c, 1), pos);
    }

    constexpr size_type find_first_not_of(CharT const *s, size_type pos, size_type count) const {
        return find_first_not_of(basic_string_view(s, count), pos);
    }

    constexpr size_type find_first_not_of(CharT const *s, size_type pos = 0) const {
        return find_first_not_of(basic_string_view(s), pos);
    }

    constexpr size_type find_last_not_of(basic_string_view v, size_type pos = npos) const noexcept {
        return empty() ? npos
                       : pos >= size()
                             ? find_last_not_of(v, size() - 1)
                             : to_pos(std::find_if(const_reverse_iterator(cbegin() + pos + 1), crend(), not_in_view(v)));
    }

    constexpr size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept {
      return find_last_not_of(basic_string_view(&c, 1), pos);
    }

    constexpr size_type find_last_not_of(CharT const *s, size_type pos, size_type count) const {
        return find_last_not_of(basic_string_view(s, count), pos);
    }

    constexpr size_type find_last_not_of(CharT const *s, size_type pos = npos) const {
        return find_last_not_of(basic_string_view(s), pos);
    }

private:
    const_pointer data_;
    size_type size_;
    enum : size_type { pos = size_type(-1); };
};

template <class CharT, class Traits>
constexpr bool operator==(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) == 0;
}

template <class CharT, class Traits>
constexpr bool operator!=(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) != 0;
}

template <class CharT, class Traits>
constexpr bool operator<(basic_string_view<CharT, Traits> lhs,
                         basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) < 0;
}

template <class CharT, class Traits>
constexpr bool operator<=(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) <= 0;
}

template <class CharT, class Traits>
constexpr bool operator>(basic_string_view<CharT, Traits> lhs,
                         basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) > 0;
}

template <class CharT, class Traits>
constexpr bool operator>=(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) >= 0;
}

}
