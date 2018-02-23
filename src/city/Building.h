#pragma once

#include "city/Tile.h"

class Building : public Tile
{
public:
    Building(const sf::Texture& texture, sf::IntRect rect, Type type, unsigned int height);
    virtual ~Building();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void updateVariant(Type neighbors[3][3]) override;
};
