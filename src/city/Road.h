#pragma once

#include "city/Tile.h"

class Road : public Tile
{
public:
    Road(const sf::Texture& texture, sf::IntRect rect, Type type, unsigned int height);
    virtual ~Road();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void updateVariant(Type neighbors[3][3]) override;
};
