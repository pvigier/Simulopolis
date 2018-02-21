#include "Tile.h"

Tile::Tile()
{

}

Tile::Tile(unsigned int height, const sf::Texture& texture,
    std::vector<Animation> animations, Tile::Type type, unsigned int cost) :
    mAnimator(animations), mType(type), mVariant(0), mCost(cost)
{
    mSprite.setOrigin(sf::Vector2f(0.0f, Tile::SIZE * (height - 1)));
    mSprite.setTexture(texture);
}

void Tile::draw(sf::RenderWindow& window, float dt)
{
    // Update the animation and sprite
    mAnimator.changeAnimation(mVariant);
    mAnimator.update(dt);
    mSprite.setTextureRect(mAnimator.getDisplayedRect());

    // Draw the tile
    window.draw(mSprite);
}

void Tile::update()
{
     /* If the population is at the maximum value for the tile,
     * there is a small chance that the tile will increase its
     * building stage */
    /*if ((mType == Tile::Type::RESIDENTIAL || mType == Tile::Type::COMMERCIAL || mType == Tile::Type::INDUSTRIAL)
        && (mPopulation == mMaxPopPerLevel * (mVariant + 1))
        && (mVariant < mMaxLevels))
    {
        if (rand() % int(1e4) < 1e2 / (mVariant+1))
            ++mVariant;
    }*/
}

sf::Sprite& Tile::getSprite()
{
    return mSprite;
}

Tile::Type& Tile::getType()
{
    return mType;
}

Tile::Type Tile::getType() const
{
    return mType;
}

unsigned int& Tile::getVariant()
{
    return mVariant;
}

unsigned int Tile::getCost() const
{
    return mCost;
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
