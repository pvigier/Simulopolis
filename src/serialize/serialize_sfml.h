#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{

template <typename Archive, typename T>
void serialize(Archive& ar, Vector2<T>& v, const unsigned int version)
{
    ar & v.x & v.y;
}

}
