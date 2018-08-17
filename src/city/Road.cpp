#include "Road.h"

Road::Road(const std::string& name, Type type) :
    Tile(name, type, Category::ROAD)
{
    sf::IntRect rect(0, 0, 132, 101);
    if (type == Type::ROAD_SIDEWALK)
        rect.left = 132;
    else if (type == Type::BRIDGE)
    {
        rect.left = 264;
        mCategory = Category::BRIDGE;
    }
    mSprite.setTextureRect(rect);
}

Road::~Road()
{
    //dtor
}

std::unique_ptr<Tile> Road::clone() const
{
    return std::unique_ptr<Tile>(new Road(*this));
}

bool Road::updateVariant(const Tile* neighbors[3][3])
{
    sf::IntRect rect = mSprite.getTextureRect();

    if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad() &&
        neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 202;
    else if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad() && neighbors[2][1]->isRoad())
        rect.top = 707;
    else if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 909;
    else if (neighbors[0][1]->isRoad() && neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 808;
    else if (neighbors[1][0]->isRoad() && neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 1010;
    else if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad())
        rect.top = 505;
    else if (neighbors[1][0]->isRoad() && neighbors[2][1]->isRoad())
        rect.top = 404;
    else if (neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 606;
    else if (neighbors[1][2]->isRoad() && neighbors[0][1]->isRoad())
        rect.top = 303;
    else if ((neighbors[1][0]->isRoad() && neighbors[1][2]->isRoad()) ||
        (neighbors[1][0]->isRoad() && !neighbors[1][2]->isRoad()) ||
        (!neighbors[1][0]->isRoad() && neighbors[1][2]->isRoad()))
        rect.top = 101;

    if (mSprite.getTextureRect() == rect)
        return false;

    mSprite.setTextureRect(rect);
    return true;
}
