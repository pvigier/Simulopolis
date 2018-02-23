#pragma once

#include "city/Tile.h"

class Building : public Tile
{
public:
    Building(const sf::Texture& texture, Type type, unsigned int height);
    virtual ~Building();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual bool updateVariant(Tile* neighbors[3][3]) override;

    virtual bool isBuilding() const override;
};
