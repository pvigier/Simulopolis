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
