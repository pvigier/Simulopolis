#include "Building.h"

Building::Building(const sf::Texture& texture, Type type) :
    Tile(texture, type), mNbStairs(3)
{
    //ctor
}

Building::~Building()
{
    //dtor
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite);
    // Stairs
    sf::Sprite sprite(mSprite);
    sprite.setTextureRect(sf::IntRect(mSprite.getTextureRect().left, 0, 132, 85));
    for (unsigned int i = 0; i < mNbStairs - 1; ++i)
    {
        sprite.move(0, -STAIR_HEIGHT);
        target.draw(sprite);
    }
}

std::unique_ptr<Tile> Building::clone() const
{
    return std::unique_ptr<Tile>(new Building(*this));
}

bool Building::updateVariant(Tile* neighbors[3][3])
{
    sf::IntRect rect(0, 85, 132, 128);

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

void Building::setPosition(sf::Vector2f position)
{
    mSprite.setPosition(sf::Vector2f(position.x, position.y - OFFSET_Y));
}
