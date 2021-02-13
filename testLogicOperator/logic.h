#include <ciso646>  // Required for some compilers to use aliases for boolean operators
#include <type_traits>

template <bool... B>
struct logic_helper;

template <bool... B>
struct all {
    using type = std::is_same<logic_helper<B...>, logic_helper<(B or true)...>>;
};

template <bool... B>
using all_t = std::is_same<logic_helper<B...>, logic_helper<(B or true)...>>;

template <bool... B>
struct any
{
    using type =
        std::integral_constant<bool, not std::is_same<logic_helper<B...>, logic_helper<(B and false)...>>::value>;
};

template <bool... B>
using any_t =
    std::integral_constant<bool, not std::is_same<logic_helper<B...>, logic_helper<(B and false)...>>::value>;

template <bool... B>
using none_t = std::is_same<logic_helper<B...>, logic_helper<(B and false)...>>;

template <bool>
struct not_impl;

template <>
struct not_impl<true>
{
    using type = std::false_type;
};

template <>
struct not_impl<false>
{
    using type = std::true_type;
};

template <template <typename> class Predicate, typename T>
using not_t = typename not_impl<Predicate<T>::value>::type;

template <typename T>
using identity_t = T;