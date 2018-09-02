#pragma once

#include "util/Array2.h"
#include "pcg/RandomGenerator.h"
#include "city/Tile.h"

class TerrainGenerator
{
public:
    TerrainGenerator(RandomGenerator& generator);

    Array2<Tile::Type> generate() const;

private:
    RandomGenerator& mGenerator;
};
