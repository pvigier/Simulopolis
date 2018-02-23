#include "Road.h"

Road::Road(const sf::Texture& texture, sf::IntRect rect, Type type, unsigned int height) :
    Tile(texture, rect, type, height), mSidewalk(false)
{
    //ctor
}

Road::~Road()
{
    //dtor
}

std::unique_ptr<Tile> Road::clone() const
{
    return std::unique_ptr<Tile>(new Road(*this));
}

bool Road::updateVariant(Tile* neighbors[3][3])
{
    sf::IntRect rect(0, 0, 132, 101);

    if (neighbors[0][1]->isBuilding() || neighbors[1][0]->isBuilding() ||
        neighbors[2][1]->isBuilding() || neighbors[1][2]->isBuilding() ||
        (neighbors[0][1]->hasSidewalk() && neighbors[1][0]->hasSidewalk() &&
        neighbors[2][1]->hasSidewalk() && neighbors[1][2]->hasSidewalk()))
    {
        rect.left = 132;
        mSidewalk = true;
    }
    else
        mSidewalk = false;

    if (neighbors[0][1]->getType() == Type::ROAD && neighbors[1][0]->getType() == Type::ROAD &&
        neighbors[2][1]->getType() == Type::ROAD && neighbors[1][2]->getType() == Type::ROAD)
        rect.top = 202;
    else if (neighbors[0][1]->getType() == Type::ROAD && neighbors[1][0]->getType() == Type::ROAD &&
        neighbors[2][1]->getType() == Type::ROAD)
        rect.top = 707;
    else if (neighbors[0][1]->getType() == Type::ROAD && neighbors[1][0]->getType() == Type::ROAD &&
         neighbors[1][2]->getType() == Type::ROAD)
        rect.top = 909;
    else if (neighbors[0][1]->getType() == Type::ROAD && neighbors[2][1]->getType() == Type::ROAD &&
        neighbors[1][2]->getType() == Type::ROAD)
        rect.top = 808;
    else if (neighbors[1][0]->getType() == Type::ROAD && neighbors[2][1]->getType() == Type::ROAD &&
        neighbors[1][2]->getType() == Type::ROAD)
        rect.top = 1010;
    else if (neighbors[0][1]->getType() == Type::ROAD && neighbors[1][0]->getType() == Type::ROAD)
        rect.top = 505;
    else if (neighbors[1][0]->getType() == Type::ROAD && neighbors[2][1]->getType() == Type::ROAD)
        rect.top = 404;
    else if (neighbors[2][1]->getType() == Type::ROAD && neighbors[1][2]->getType() == Type::ROAD)
        rect.top = 606;
    else if (neighbors[1][2]->getType() == Type::ROAD && neighbors[0][1]->getType() == Type::ROAD)
        rect.top = 303;
    else if ((neighbors[1][0]->getType() == Type::ROAD && neighbors[1][2]->getType() == Type::ROAD) ||
        (neighbors[1][0]->getType() == Type::ROAD && neighbors[1][2]->getType() != Type::ROAD) ||
        (neighbors[1][0]->getType() != Type::ROAD && neighbors[1][2]->getType() == Type::ROAD))
        rect.top = 101;

    if (mSprite.getTextureRect() == rect)
        return false;

    mSprite.setTextureRect(rect);
    return true;
}

bool Road::hasSidewalk() const
{
    return mSidewalk;
}
