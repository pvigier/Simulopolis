#include "Network.h"
#include <queue>
#include <algorithm>

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
    mEntryPoints.insert(std::make_pair(i, j));
}

void Network::removeRoad(int i, int j)
{
    mRoads.set(i, j, false);
    mEntryPoints.erase(std::make_pair(i, j));
}

bool Network::getAdjacentRoad(int i, int j, sf::Vector2i& coords) const
{
    if (i < 0 || j < 0 || i >= static_cast<int>(mRoads.getHeight()) || j >= static_cast<int>(mRoads.getWidth()))
        return false;
    else if (i > 0 && mRoads.get(i - 1, j))
    {
        coords = sf::Vector2i(i - 1, j);
        return true;
    }
    else if (j + 1 < static_cast<int>(mRoads.getWidth()) && mRoads.get(i, j + 1))
    {
        coords = sf::Vector2i(i, j + 1);
        return true;
    }
    else if (j > 0 && mRoads.get(i, j - 1))
    {
        coords = sf::Vector2i(i, j - 1);
        return true;
    }
    else if (i + 1 < static_cast<int>(mRoads.getHeight()) && mRoads.get(i + 1, j))
    {
        coords = sf::Vector2i(i + 1, j);
        return true;
    }
    else
        return false;
}

bool Network::getRandomEntryPoint(sf::Vector2i& coords) const
{
    if (mEntryPoints.empty())
        return false;
    else
    {
        std::uniform_int_distribution<int> entryPointPdf(0, mEntryPoints.size() - 1);
        auto it(mEntryPoints.begin());
        std::advance(it, entryPointPdf(mGenerator));
        coords = sf::Vector2i(it->first, it->second);
        return true;
    }
}

std::vector<sf::Vector2i> Network::getPath(sf::Vector2i start, sf::Vector2i end) const
{
    // Inverse the start and the end, because we will find a path from end to start
    std::swap(start, end);

    // BFS
    std::queue<sf::Vector2i> frontier;
    Array2<int> states(mRoads.getHeight(), mRoads.getWidth(), -1);
    frontier.push(start);
    while (!frontier.empty() && frontier.front() != end) {
        sf::Vector2i position = frontier.front();
        frontier.pop();
        for (int i = 0; i < 4; ++i)
        {
            sf::Vector2i neighbor = position + sDirections[i];
            if (neighbor.y >= 0 && neighbor.y < static_cast<int>(mRoads.getHeight()) &&
                neighbor.x >= 0 && neighbor.x < static_cast<int>(mRoads.getWidth()) &&
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
