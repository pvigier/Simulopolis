#pragma once

#include "city/Tile.h"

class Road : public Tile
{
public:
    Road(const sf::Texture& texture, Type type);
    virtual ~Road();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual bool updateVariant(Tile* neighbors[3][3]) override;

    virtual bool hasSidewalk() const override;

protected:
    bool mSidewalk;
};
