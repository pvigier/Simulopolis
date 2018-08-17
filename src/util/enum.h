#pragma once

template<typename E>
struct EnableBitMaskOperators
{
    static constexpr bool enable = false;
};

template<typename E>
typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type constexpr none = static_cast<E>(0);

template<typename E>
typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type constexpr all = static_cast<E>(~0);

template<typename E>
typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type operator~(E e)
{
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(~static_cast<underlying>(e));
}

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

#define ENABLE_BITMASK_OPERATORS(E)                        \
template<>                                                 \
struct EnableBitMaskOperators<E>                           \
{                                                          \
    static constexpr bool enable = std::is_enum<E>::value; \
};
