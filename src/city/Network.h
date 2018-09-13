#pragma once

#include <array>
#include <vector>
#include <map>
#include <random>
#include <SFML/System/Vector2.hpp>
#include "util/Array2.h"
#include "serialize/serialize_sfml.h"

class Network
{
public:
    Network(unsigned int width = 0, unsigned int height = 0);

    void reshape(unsigned int width, unsigned int height);

    void addRoad(int i, int j);
    void removeRoad(int i, int j);
    void updateComponents();
    bool getAdjacentRoad(int i, int j, sf::Vector2i& coords) const;
    bool getRandomEntryPoint(int i, int j, sf::Vector2i& coords) const;
    bool isReachableFrom(sf::Vector2i start, sf::Vector2i end) const;
    std::vector<sf::Vector2i> getPath(sf::Vector2i start, sf::Vector2i end) const;

private:
    static const std::array<sf::Vector2i, 4> sDirections;

    mutable std::default_random_engine mGenerator;
    Array2<bool> mRoads;
    Array2<int> mComponents;
    std::map<int, std::vector<sf::Vector2i>> mEntryPoints;

    void floodFill(int i, int j, int c);
    bool isValid(int i, int j) const;
    bool isEntryPoint(int i, int j) const;

    // Serialization
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mRoads & mComponents & mEntryPoints;
    }
};
