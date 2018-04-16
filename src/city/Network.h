#pragma once

#include <vector>
#include <queue>
#include <SFML/System/Vector2.hpp>

class Network
{
public:
    Network(unsigned int width, unsigned int height);

    void resize(unsigned int width, unsigned int height);

    void addRoad(sf::Vector2i position);
    void removeRoad(sf::Vector2i position);
    std::vector<sf::Vector2i> getPath(sf::Vector2i start, sf::Vector2i end) const;

private:
    static const sf::Vector2i sDirections[4];

    unsigned int mWidth;
    unsigned int mHeight;
    std::vector<bool> mRoads;

    inline std::size_t positionToIndex(sf::Vector2i position) const;
};
