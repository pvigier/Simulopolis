#include "Road.h"

Road::Road(const sf::Texture& texture, sf::IntRect rect, Type type, unsigned int height) :
    Tile(texture, rect, type, height)
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

void Road::updateVariant(Type neighbors[3][3])
{
    sf::IntRect rect(0, 0, 132, 99);

    if (neighbors[0][1] == Type::RESIDENTIAL || neighbors[1][0] == Type::RESIDENTIAL ||
        neighbors[2][1] == Type::RESIDENTIAL || neighbors[1][2] == Type::RESIDENTIAL)
        rect.left = 132;

    if (neighbors[0][1] == Type::ROAD && neighbors[1][0] == Type::ROAD &&
        neighbors[2][1] == Type::ROAD && neighbors[1][2] == Type::ROAD)
        rect.top = 202;
    else if (neighbors[0][1] == Type::ROAD && neighbors[1][0] == Type::ROAD &&
        neighbors[2][1] == Type::ROAD)
        rect.top = 707;
    else if (neighbors[0][1] == Type::ROAD && neighbors[1][0] == Type::ROAD &&
         neighbors[1][2] == Type::ROAD)
        rect.top = 909;
    else if (neighbors[0][1] == Type::ROAD && neighbors[2][1] == Type::ROAD &&
        neighbors[1][2] == Type::ROAD)
        rect.top = 808;
    else if (neighbors[1][0] == Type::ROAD && neighbors[2][1] == Type::ROAD &&
        neighbors[1][2] == Type::ROAD)
        rect.top = 1010;
    else if (neighbors[0][1] == Type::ROAD && neighbors[1][0] == Type::ROAD)
        rect.top = 505;
    else if (neighbors[1][0] == Type::ROAD && neighbors[2][1] == Type::ROAD)
        rect.top = 404;
    else if (neighbors[2][1] == Type::ROAD && neighbors[1][2] == Type::ROAD)
        rect.top = 606;
    else if (neighbors[1][2] == Type::ROAD && neighbors[0][1] == Type::ROAD)
        rect.top = 303;
    else if ((neighbors[1][0] == Type::ROAD && neighbors[1][2] == Type::ROAD) ||
        (neighbors[1][0] == Type::ROAD && neighbors[1][2] != Type::ROAD) ||
        (neighbors[1][0] != Type::ROAD && neighbors[1][2] == Type::ROAD))
        rect.top = 101;
    mSprite.setTextureRect(rect);
}
