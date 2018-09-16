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

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{

template <typename Archive, typename T>
void serialize(Archive& ar, Vector2<T>& v, const unsigned int /*version*/)
{
    ar & v.x & v.y;
}

template<typename Archive>
void serialize(Archive& ar, Color& c, const unsigned int /*version*/)
{
    ar & c.r & c.g & c.b & c.a;
}

}
