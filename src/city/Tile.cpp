#include "Tile.h"

Tile::Tile(const sf::Texture& texture, sf::IntRect rect, Tile::Type type, unsigned int height) :
    mSprite(texture), mType(type)
{
    mSprite.setTextureRect(rect);
    mSprite.setOrigin(sf::Vector2f(0.0f, Tile::SIZE * (height - 1)));
}

Tile::~Tile()
{

}

std::unique_ptr<Tile> Tile::clone() const
{
    return std::unique_ptr<Tile>(new Tile(*this));
}

bool Tile::updateVariant(Tile* neighbors[3][3])
{
    return false;
}

bool Tile::isBuilding() const
{
    return false;
}

bool Tile::hasSidewalk() const
{
    return false;
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
