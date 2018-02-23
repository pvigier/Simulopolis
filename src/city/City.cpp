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

void City::update(float dt)
{

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

unsigned int City::getDay() const
{
    return mDay;
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
