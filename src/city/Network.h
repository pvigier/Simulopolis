#pragma once

#include <vector>
#include <set>
#include <utility>
#include <random>
#include <SFML/System/Vector2.hpp>
#include "util/Array2.h"

class Network
{
public:
    Network(unsigned int width, unsigned int height);

    void reshape(unsigned int width, unsigned int height);

    void addRoad(int i, int j);
    void removeRoad(int i, int j);
    bool getAdjacentRoad(int i, int j, sf::Vector2i& coords) const;
    bool getRandomEntryPoint(sf::Vector2i& coords) const;
    std::vector<sf::Vector2i> getPath(sf::Vector2i start, sf::Vector2i end) const;

private:
    mutable std::default_random_engine mGenerator;
    static const sf::Vector2i sDirections[4];
    Array2<bool> mRoads;
    std::set<std::pair<int, int>> mEntryPoints;
};
