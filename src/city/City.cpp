#include "City.h"
#include <fstream>
#include <sstream>
#include <iostream>

City::City() :
    mCurrentTime(0.0), mTimePerDay(1.0), mDay(0), mPopulation(0), mUnemployed(0), mFunds(0)
{

}

City::City(std::string cityName) : City()
{
    load(cityName);

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

    mCarsByTile.resize(mMap.getWidth() * mMap.getHeight());
}

void City::load(std::string cityName)
{
    int width = 0;
    int height = 0;

    std::ifstream inputFile(cityName + "_cfg.dat", std::ios::in);
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

    mMap.load(cityName + "_map.dat", width, height);
}

void City::save(std::string cityName)
{
    std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);

    outputFile << "width=" << mMap.getWidth() << std::endl;
    outputFile << "height=" << mMap.getHeight() << std::endl;
    outputFile << "day=" << mDay << std::endl;
    outputFile << "population=" << mPopulation << std::endl;
    outputFile << "unemployed=" << mUnemployed << std::endl;
    outputFile << "funds=" << mFunds << std::endl;

    outputFile.close();

    mMap.save(cityName + "_map.dat");
}

void City::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    const std::vector<std::unique_ptr<Tile>>& tiles = mMap.getTiles();
    for (unsigned int y = 0; y < mMap.getHeight(); ++y)
    {
        for (unsigned int x = 0; x < mMap.getWidth(); ++x)
        {
            std::size_t i = y * mMap.getWidth() + x;
            target.draw(*tiles[i]);
            for (const Car* car : mCarsByTile[i])
                target.draw(*car);
        }
    }
}

void City::update(float dt)
{
    for (Car& car : mCars)
        car.update(dt);

    for (unsigned int y = 0; y < mMap.getHeight(); ++y)
    {
        for (unsigned int x = 0; x < mMap.getWidth(); ++x)
        {
            std::size_t i = y * mMap.getWidth() + x;
            mCarsByTile[i].clear();
        }
    }
    for (const Car& car : mCars)
    {
        sf::Vector2f bottomLeft(car.getBounds().left, car.getBounds().top + car.getBounds().height);
        sf::Vector2f bottomRight(bottomLeft.x + car.getBounds().width, bottomLeft.y);
        sf::Vector2i iBottomLeft = toTileIndices(bottomLeft);
        sf::Vector2i iBottomRight = toTileIndices(bottomRight);
        sf::Vector2i indices(std::max(iBottomLeft.x, iBottomRight.x), std::max(iBottomLeft.y, iBottomRight.y));
        mCarsByTile[indices.y * mMap.getWidth() + indices.x].push_back(&car);
    }
    for (unsigned int y = 0; y < mMap.getHeight(); ++y)
    {
        for (unsigned int x = 0; x < mMap.getWidth(); ++x)
        {
            std::size_t i = y * mMap.getWidth() + x;
            std::sort(mCarsByTile[i].begin(), mCarsByTile[i].end(), [](const Car* car1, const Car* car2) { return car1->getBounds().top < car2->getBounds().top; });
        }
    }
}

void City::bulldoze(Tile::Type type)
{
    mMap.bulldoze(type);
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
