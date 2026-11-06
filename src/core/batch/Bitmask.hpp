#pragma once

#include <type_traits>

// default disable bitmask for all enums
template <typename E>
inline constexpr bool enable_bitmask = false;

// concept = enum && enabled bitmask
template <typename E>
concept BitmaskEnum = std::is_enum_v<E> && enable_bitmask<E>;

template <BitmaskEnum E>
constexpr E operator|(E a, E b)
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) | static_cast<U>(b));
}

template <BitmaskEnum E>
constexpr E operator&(E a, E b)
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) & static_cast<U>(b));
}

template <BitmaskEnum E>
constexpr E operator~(E a)
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<U>(a));
}

template <BitmaskEnum E>
constexpr E& operator|=(E& a, E b)
{
    a = a | b;
    return a;
}

template <BitmaskEnum E>
constexpr E& operator&=(E& a, E b)
{
    a = a & b;
    return a;
}
