/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

#include <type_traits>

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
