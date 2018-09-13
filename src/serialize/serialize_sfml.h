#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{

template <typename Archive, typename T>
void serialize(Archive& ar, Vector2<T>& v, const unsigned int version)
{
    ar & v.x & v.y;
}

template <typename Archive>
void serialize(Archive& ar, Color& c, const unsigned int version)
{
    ar & c.r & c.g & c.b & c.a;
}

}
