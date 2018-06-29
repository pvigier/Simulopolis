#pragma once

template<typename E>
struct EnableBitMaskOperators
{
    static const bool enable = false; // constexpr
};

template<typename E>
typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type operator|(E lhs, E rhs)
{
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type operator&(E lhs, E rhs)
{
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if<EnableBitMaskOperators<E>::enable, bool>::type any(E e)
{
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<bool>(static_cast<underlying>(e));
}

#define ENABLE_BITMASK_OPERATORS(x)  \
template<>                           \
struct EnableBitMaskOperators<x>     \
{                                    \
    static const bool enable = true; \
};
