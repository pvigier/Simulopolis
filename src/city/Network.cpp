#include "Network.h"

const sf::Vector2i Network::sDirections[4] = {sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -1), sf::Vector2i(0, 1)};

Network::Network(unsigned int width, unsigned int height) : mRoads(height, width, false)
{

}

void Network::reshape(unsigned int width, unsigned int height)
{
    mRoads.reshape(height, width, false);
}

void Network::addRoad(int i, int j)
{
    mRoads.set(i, j, true);
}

void Network::removeRoad(int i, int j)
{
    mRoads.set(i, j, false);
}

std::vector<sf::Vector2i> Network::getPath(sf::Vector2i start, sf::Vector2i end) const
{
    // Inverse the start and the end, because we will find a path from end to start
    std::swap(start, end);

    // BFS
    std::queue<sf::Vector2i> frontier;
    Array2<int> states(mRoads.getWidth(), mRoads.getHeight(), -1);
    frontier.push(start);
    while (!frontier.empty() && frontier.front() != end) {
        sf::Vector2i position = frontier.front();
        frontier.pop();
        for (int i = 0; i < 4; ++i)
        {
            sf::Vector2i neighbor = position + sDirections[i];
            if (neighbor.y >= 0 && neighbor.y < mRoads.getWidth() &&
                neighbor.x >= 0 && neighbor.x < mRoads.getHeight() &&
                mRoads.get(neighbor.y, neighbor.x) && states.get(neighbor.y, neighbor.x) == -1)
            {
                frontier.push(neighbor);
                states.set(neighbor.y, neighbor.x, i);
            }
        }
    }

    // Retrieve path
    std::vector<sf::Vector2i> points;
    if (states.get(end.y, end.x) != -1)
    {
        sf::Vector2i point = end;
        while (point != start)
        {
            points.push_back(point);
            point -= sDirections[states.get(point.y, point.x)];
        }
        points.push_back(start);
    }

    return points;
}
