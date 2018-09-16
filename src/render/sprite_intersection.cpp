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
 
#include "sprite_intersection.h"

bool sprite_intersect(const sf::Sprite& sprite, const sf::Image& mask, const sf::Vector2f& position)
{
    if (sprite.getGlobalBounds().contains(position))
    {
        sf::Vector2i texturePosition(sprite.getInverseTransform().transformPoint(position));
        texturePosition += sf::Vector2i(sprite.getTextureRect().left, sprite.getTextureRect().top);
        sf::Vector2i maskSize(mask.getSize());
        if (texturePosition.x >= 0 && texturePosition.x < maskSize.x &&
            texturePosition.y >= 0 && texturePosition.y < maskSize.y)
            return mask.getPixel(texturePosition.x, texturePosition.y).a != 0;
    }
    return false;
}
