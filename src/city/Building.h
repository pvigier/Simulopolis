#pragma once

#include <vector>
#include "city/Tile.h"

class Building : public Tile
{
public:
    static constexpr int STAIR_HEIGHT = 33;
    static constexpr int OFFSET_Y = 26;

    Building(const sf::Texture& texture, Type type);
    virtual ~Building();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual std::unique_ptr<Tile> clone() const override;

    virtual bool updateVariant(Tile* neighbors[3][3]) override;

    virtual bool isBuilding() const override;
    virtual void setPosition(sf::Vector2f position) override;

private:
    unsigned int mNbStairs;
};
