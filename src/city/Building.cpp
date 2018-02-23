#include "Building.h"

Building::Building(const sf::Texture& texture, Type type, unsigned int height) :
    Tile(texture, type, height)
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

    if (neighbors[0][1]->getType() == Type::ROAD)
        rect.left = 264;
    else if (neighbors[1][2]->getType() == Type::ROAD)
        rect.left = 132;
    else if (neighbors[1][0]->getType() == Type::ROAD)
        rect.left = 396;

    if (mSprite.getTextureRect() == rect)
        return false;

    mSprite.setTextureRect(rect);
    return true;
}

bool Building::isBuilding() const
{
    return true;
}
