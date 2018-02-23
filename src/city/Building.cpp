#include "Building.h"

Building::Building(const sf::Texture& texture, sf::IntRect rect, Type type, unsigned int height) :
    Tile(texture, rect, type, height)
{
    //ctor
}

Building::~Building()
{
    //dtor
}

std::unique_ptr<Tile> Building::clone() const
{
    return std::unique_ptr<Tile>(new Building(*this));
}

bool Building::updateVariant(Tile* neighbors[3][3])
{
    sf::IntRect rect(0, 0, 132, 165);
    mSprite.setTextureRect(rect);
    return false;
}

bool Building::isBuilding() const
{
    return true;
}
