#include "Network.h"

const sf::Vector2i Network::sDirections[4] = {sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -1), sf::Vector2i(0, 1)};

Network::Network(unsigned int width, unsigned int height) :
    mWidth(width), mHeight(height), mRoads(mWidth * mHeight, false)
{

}

void Network::resize(unsigned int width, unsigned int height)
{
    mRoads.resize(width * height, false);
    mWidth = width;
    mHeight = height;
}

void Network::addRoad(sf::Vector2i position)
{
    mRoads[positionToIndex(position)] = true;
}

void Network::removeRoad(sf::Vector2i position)
{
    mRoads[positionToIndex(position)] = false;
}

std::vector<sf::Vector2i> Network::getPath(sf::Vector2i start, sf::Vector2i end) const
{
    // Inverse the start and the end, because we will find a path from end to start
    std::swap(start, end);

    // BFS
    std::queue<sf::Vector2i> frontier;
    std::vector<int> states(mWidth * mHeight, -1);
    frontier.push(start);
    while (!frontier.empty() && frontier.front() != end) {
        sf::Vector2i position = frontier.front();
        frontier.pop();
        for (int i = 0; i < 4; ++i)
        {
            sf::Vector2i neighbor = position + sDirections[i];
            std::size_t iNeighbor = positionToIndex(neighbor);
            if (iNeighbor < mRoads.size() && mRoads[iNeighbor] && states[iNeighbor] == -1)
            {
                frontier.push(neighbor);
                states[iNeighbor] = i;
            }
        }
    }

    // Retrieve path
    std::vector<sf::Vector2i> points;
    if (states[positionToIndex(end)] != -1)
    {
        sf::Vector2i point = end;
        while (point != start)
        {
            points.push_back(point);
            std::size_t iPoint = positionToIndex(point);
            point -= sDirections[states[iPoint]];
        }
        points.push_back(start);
    }

    return points;
}

std::size_t Network::positionToIndex(sf::Vector2i position) const
{
    return position.x + position.y * mWidth;
}
