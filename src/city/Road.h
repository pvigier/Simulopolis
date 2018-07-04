#pragma once

#include "city/Tile.h"

class Road : public Tile
{
public:
    Road(const std::string& name, Type type);
    virtual ~Road();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual bool updateVariant(const Tile* neighbors[3][3]) override;

protected:
};
