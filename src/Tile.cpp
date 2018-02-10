#include "Tile.h"

Tile::Tile()
{

}

Tile::Tile(unsigned int height, const sf::Texture& texture,
    std::vector<Animation> animations, TileType type, unsigned int cost,
    unsigned int maxPopPerLevel, unsigned int maxLevels) :
    mAnimationHandler(animations, sf::IntRect(0, 0, 2 * TILE_SIZE, TILE_SIZE * height)),
    mType(type), mVariant(0), mCost(cost), mPopulation(0), mMaxPopPerLevel(maxPopPerLevel),
    mMaxLevels(maxLevels), mProduction(0), mStoredGoods(0)
{
    mRegions[0] = 0;
    mSprite.setOrigin(sf::Vector2f(0.0f, TILE_SIZE * (height - 1)));
    mSprite.setTexture(texture);
}

void Tile::draw(sf::RenderWindow& window, float dt)
{
    // Update the animation and sprite
    mAnimationHandler.changeAnimation(mVariant);
    mAnimationHandler.update(dt);
    mSprite.setTextureRect(mAnimationHandler.getDisplayedRect());

    // Draw the tile
    window.draw(mSprite);
}

void Tile::update()
{
     /* If the population is at the maximum value for the tile,
     * there is a small chance that the tile will increase its
     * building stage */
    if ((mType == TileType::RESIDENTIAL || mType == TileType::COMMERCIAL || mType == TileType::INDUSTRIAL)
        && (mPopulation == mMaxPopPerLevel * (mVariant + 1))
        && (mVariant < mMaxLevels))
    {
        if (rand() % int(1e4) < 1e2 / (mVariant+1))
            ++mVariant;
    }
}

sf::Sprite& Tile::getSprite()
{
    return mSprite;
}

TileType& Tile::getType()
{
    return mType;
}

TileType Tile::getType() const
{
    return mType;
}

unsigned int& Tile::getVariant()
{
    return mVariant;
}

unsigned int* Tile::getRegions()
{
    return mRegions;
}

unsigned int Tile::getCost() const
{
    return mCost;
}

double& Tile::getPopulation()
{
    return mPopulation;
}

double Tile::getPopulation() const
{
    return mPopulation;
}

unsigned int Tile::getMaxPopPerLevel() const
{
    return mMaxPopPerLevel;
}

float& Tile::getProduction()
{
    return mProduction;
}

float& Tile::getStoredGoods()
{
    return mStoredGoods;
}

std::string tileTypeToStr(TileType type)
{
    switch (type)
    {
        case TileType::VOID:
            return "Void";
        case TileType::GRASS:
            return "Flatten";
        case TileType::FOREST:
            return "Forest";
        case TileType::WATER:
            return "Water";
        case TileType::RESIDENTIAL:
            return "Residential Zone";
        case TileType::COMMERCIAL:
            return "Commercial Zone";
        case TileType::INDUSTRIAL:
            return "Industrial Zone";
        default:
            return "";
    }
}
