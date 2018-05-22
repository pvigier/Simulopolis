#pragma once

#include <vector>
#include <queue>
#include <SFML/System/Vector2.hpp>
#include "util/Array2.h"

class Network
{
public:
    Network(unsigned int width, unsigned int height);

    void reshape(unsigned int width, unsigned int height);

    void addRoad(int i, int j);
    void removeRoad(int i, int j);
    std::vector<sf::Vector2i> getPath(sf::Vector2i start, sf::Vector2i end) const;

private:
    static const sf::Vector2i sDirections[4];
    Array2<bool> mRoads;
};
