#pragma once

#include "util/Array2.h"
#include "city/Tile.h"
#include "noise.h"

class TerrainGenerator
{
public:
    TerrainGenerator();

    Array2<Tile::Type> generate(unsigned int seed) const;
};
