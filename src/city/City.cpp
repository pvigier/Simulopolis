#include "City.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Building.h"

City::Intersection::Intersection() : type(City::Intersection::Type::NONE), car(nullptr)
{

}

City::Intersection::Intersection(const Car* car) : type(City::Intersection::Type::CAR), car(car)
{

}

City::Intersection::Intersection(const Building* building) : type(City::Intersection::Type::BUILDING), building(building)
{

}

City::City() : mCurrentTime(0.0), mTimePerDay(1.0), mDay(0), mPopulation(0), mUnemployed(0), mFunds(0)
{

}

void City::load(const std::string& name)
{
    int width = 0;
    int height = 0;

    std::ifstream inputFile(name + "_cfg.dat", std::ios::in);
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream lineStream(line);
        std::string key;
        if(std::getline(lineStream, key, '='))
        {
            std::string value;
            if(std::getline(lineStream, value))
            {
                if(key == "width")
                    width = std::stoi(value);
                else if(key == "height")
                    height = std::stoi(value);
                else if(key == "day")
                    mDay = std::stoi(value);
                else if(key == "unemployed")
                    mUnemployed = std::stod(value);
                else if(key == "population")
                    mPopulation = std::stod(value);
                else if(key == "funds")
                    mFunds = std::stod(value);
            }
            else
                std::cerr << "Error, no value for key " << key << std::endl;
        }
    }

    inputFile.close();

    mMap.load(name + "_map.dat", width, height);
    mCarsByTile.reshape(mMap.getHeight(), mMap.getWidth());

    // Tests
    mMap.select(sf::Vector2i(0, 0), sf::Vector2i(0, 10), {});
    mMap.select(sf::Vector2i(1, 10), sf::Vector2i(10, 10), {});
    mMap.select(sf::Vector2i(10, 9), sf::Vector2i(10, 0), {});
    mMap.select(sf::Vector2i(9, 0), sf::Vector2i(2, 0), {});
    bulldoze(Tile::Type::ROAD_GRASS);

    mCars.reserve(2);

    mCars.emplace_back("car_blue_sedan_2");
    Path path = mMap.getPath(sf::Vector2i(0, 0), sf::Vector2i(2, 0));
    mCars.back().getKinematic().setPosition(path.getCurrentPoint());
    mCars.back().getSteering().setPath(path);

    mCars.emplace_back("car_blue_sedan_2");
    Path otherPath = mMap.getPath(sf::Vector2i(2, 0), sf::Vector2i(0, 0));
    mCars.back().getKinematic().setPosition(otherPath.getCurrentPoint());
    mCars.back().getSteering().setPath(otherPath);
}

void City::save(const std::string& name)
{
    std::ofstream outputFile(name + "_cfg.dat", std::ios::out);

    outputFile << "width=" << mMap.getWidth() << std::endl;
    outputFile << "height=" << mMap.getHeight() << std::endl;
    outputFile << "day=" << mDay << std::endl;
    outputFile << "population=" << mPopulation << std::endl;
    outputFile << "unemployed=" << mUnemployed << std::endl;
    outputFile << "funds=" << mFunds << std::endl;

    outputFile.close();

    mMap.save(name + "_map.dat");
}

void City::createMap(const Array2<Tile::Type>& tiles)
{
    mMap.fromArray(tiles);
    mCarsByTile.reshape(mMap.getHeight(), mMap.getWidth());
}

void City::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    const Array2<std::unique_ptr<Tile>>& tiles = mMap.getTiles();
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
        {
            target.draw(*tiles.get(i, j));
            for (const Car* car : mCarsByTile.get(i, j))
                target.draw(*car);
        }
    }
}

void City::update(float dt)
{
    for (Car& car : mCars)
        car.update(dt);

    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
            mCarsByTile.get(i, j).clear();
    }
    for (const Car& car : mCars)
    {
        sf::Vector2f bottomLeft(car.getBounds().left, car.getBounds().top + car.getBounds().height);
        sf::Vector2f bottomRight(bottomLeft.x + car.getBounds().width, bottomLeft.y);
        sf::Vector2i iBottomLeft = toTileIndices(bottomLeft);
        sf::Vector2i iBottomRight = toTileIndices(bottomRight);
        sf::Vector2i indices(std::max(iBottomLeft.x, iBottomRight.x), std::max(iBottomLeft.y, iBottomRight.y));
        mCarsByTile.get(indices.y, indices.x).push_back(&car);
    }
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
            std::sort(mCarsByTile.get(i, j).begin(), mCarsByTile.get(i, j).end(), [](const Car* car1, const Car* car2) { return car1->getBounds().top < car2->getBounds().top; });
    }
}

void City::bulldoze(Tile::Type type)
{
    mMap.bulldoze(type);
}

City::Intersection City::intersect(const sf::Vector2f& position)
{
    Intersection intersection;

    const Array2<std::unique_ptr<Tile>>& tiles = mMap.getTiles();
    for (unsigned int i = 0; i < mMap.getHeight(); ++i)
    {
        for (unsigned int j = 0; j < mMap.getWidth(); ++j)
        {
            const Tile* tile = tiles.get(i, j).get();
            if (tile->intersect(position))
            {
                if (tile->isBuilding())
                    intersection = Intersection(static_cast<const Building*>(tile));
                else
                    intersection = Intersection();
            }
            for (const Car* car : mCarsByTile.get(i, j))
            {
                if (car->intersect(position))
                    intersection = Intersection(car);
            }
        }
    }
    return intersection;
}

Map& City::getMap()
{
    return mMap;
}

const Map& City::getMap() const
{
    return mMap;
}

std::vector<Car>& City::getCars()
{
    return mCars;
}

unsigned int City::getDay() const
{
    return mDay;
}

unsigned int City::getYear() const
{
    return mDay / 365;
}

unsigned int City::getPopulation() const
{
    return mPopulation;
}

unsigned int City::getUnemployed() const
{
    return mUnemployed;
}

unsigned int City::getFunds() const
{
    return mFunds;
}

void City::decreaseFunds(unsigned int amount)
{
    mFunds -= amount;
}

sf::Vector2i City::toTileIndices(const Vector2f& position) const
{
    int x = position.y / Tile::SIZE + 0.5f * (position.x / Tile::SIZE - mMap.getWidth() - 1);
    int y = position.y / Tile::SIZE - 0.5f * (position.x / Tile::SIZE - mMap.getWidth() - 1);
    return sf::Vector2i(x, y);
}
