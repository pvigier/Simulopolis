#include "Tile.h"

Tile::Tile()
{

}

Tile::Tile(const sf::Texture& texture, sf::IntRect rect, Tile::Type type, unsigned int height) :
    mSprite(texture), mType(type)
{
    mSprite.setTextureRect(rect);
    mSprite.setOrigin(sf::Vector2f(0.0f, Tile::SIZE * (height - 1)));
}

sf::Sprite& Tile::getSprite()
{
    return mSprite;
}

Tile::Type Tile::getType() const
{
    return mType;
}

std::string tileTypeToStr(Tile::Type type)
{
    switch (type)
    {
        case Tile::Type::VOID:
            return "Void";
        case Tile::Type::GRASS:
            return "Flatten";
        case Tile::Type::FOREST:
            return "Forest";
        case Tile::Type::WATER:
            return "Water";
        case Tile::Type::RESIDENTIAL:
            return "Residential Zone";
        case Tile::Type::COMMERCIAL:
            return "Commercial Zone";
        case Tile::Type::INDUSTRIAL:
            return "Industrial Zone";
        case Tile::Type::ROAD:
            return "Road";
        default:
            return "";
    }
}
