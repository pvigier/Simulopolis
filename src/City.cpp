#include "City.h"
#include <fstream>
#include <sstream>
#include <iostream>

City::City() :
    mCurrentTime(0.0), mTimePerDay(1.0), mDay(0),
    mPopulationPool(0), mPopulation(mPopulationPool),
    mEmploymentPool(0), mEmployable(mEmploymentPool), mPropCanWork(0.5),
    mBirthRate(0.00055), mDeathRate(0.00023),
    mResidentialTax(0.05), mCommercialTax(0.05), mIndustrialTax(0.05),
    mEarnings(0), mFunds(0)
{

}

City::City(std::string cityName, int tileSize, std::map<std::string, Tile>& tileAtlas) : City()
{
    mMap.setTileSize(tileSize);
    load(cityName, tileAtlas);
}

void City::load(std::string cityName, std::map<std::string, Tile>& tileAtlas)
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
                else if(key == "populationPool")
                    mPopulationPool = std::stod(value);
                else if(key == "employmentPool")
                    mEmploymentPool = std::stod(value);
                else if(key == "population")
                    mPopulation = std::stod(value);
                else if(key == "employable")
                    mEmployable = std::stod(value);
                else if(key == "birthRate")
                    mBirthRate = std::stod(value);
                else if(key == "deathRate")
                    mDeathRate = std::stod(value);
                else if(key == "residentialTax")
                    mResidentialTax = std::stod(value);
                else if(key == "commercialTax")
                    mCommercialTax = std::stod(value);
                else if(key == "industrialTax")
                    mIndustrialTax = std::stod(value);
                else if(key == "funds")
                    mFunds = std::stod(value);
                else if(key == "earnings")
                    mEarnings = std::stod(value);
            }
            else
                std::cerr << "Error, no value for key " << key << std::endl;
        }
    }

    inputFile.close();

    mMap.load(cityName + "_map.dat", width, height, tileAtlas);
    tileChanged();
}

void City::save(std::string cityName)
{
    std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);

    outputFile << "width=" << mMap.getWidth() << std::endl;
    outputFile << "height=" << mMap.getHeight() << std::endl;
    outputFile << "day=" << mDay << std::endl;
    outputFile << "populationPool=" << mPopulationPool << std::endl;
    outputFile << "employmentPool=" << mEmploymentPool << std::endl;
    outputFile << "population=" << mPopulation << std::endl;
    outputFile << "employable=" << mEmployable << std::endl;
    outputFile << "birthRate=" << mBirthRate << std::endl;
    outputFile << "deathRate=" << mDeathRate << std::endl;
    outputFile << "residentialTax=" << mResidentialTax << std::endl;
    outputFile << "commercialTax=" << mCommercialTax << std::endl;
    outputFile << "industrialTax=" << mIndustrialTax << std::endl;
    outputFile << "funds=" << mFunds << std::endl;
    outputFile << "earnings=" << mEarnings << std::endl;

    outputFile.close();

    mMap.save(cityName + "_map.dat");
}

void City::bulldoze(const Tile& tile)
{
    // Replace the selected tiles on the map with the tile and
    // update populations etc accordingly
    for (unsigned int pos = 0; pos < mMap.getWidth() * mMap.getHeight(); ++pos)
    {
        if (mMap.getTileState(pos) == TileState::SELECTED)
        {
            const Tile& tileToDelete = mMap.getTile(pos);
            TileType type = tileToDelete.getType();
            if (type == TileType::RESIDENTIAL)
                mPopulationPool += tileToDelete.getPopulation();
            else if (type == TileType::COMMERCIAL || type == TileType::INDUSTRIAL)
                mEmploymentPool += tileToDelete.getPopulation();
            mMap.setTile(pos, tile);
        }
    }
}

void City::shuffleTiles()
{
    mShuffledTiles.resize(mMap.getWidth() * mMap.getHeight());
    std::iota(mShuffledTiles.begin(), mShuffledTiles.end(), 1);
    std::random_shuffle(mShuffledTiles.begin(), mShuffledTiles.end());
}

void City::tileChanged()
{
    mMap.updateDirection(TileType::ROAD);
    mMap.findConnectedRegions(
        {TileType::ROAD, TileType::RESIDENTIAL, TileType::COMMERCIAL, TileType::INDUSTRIAL}, 0);
}

double City::getHomeless()
{
    return mPopulationPool;
}

double City::getUnemployed()
{
    return mEmploymentPool;
}

double City::distributePool(double& pool, Tile& tile, double rate)
{
    const static int moveRate = 4;

    unsigned int maxPop = tile.getMaxPopPerLevel() * (tile.getVariant() + 1);

    // If there is room in the zone, move up to 4 people from the
    // pool into the zone
    if (pool > 0)
    {
        int moving = maxPop - tile.getPopulation();
        if (moving > moveRate)
            moving = moveRate;
        if (pool < moving)
            moving = pool;
        pool -= moving;
        tile.getPopulation() += moving;
    }

    // Adjust the tile population for births and deaths
    tile.getPopulation() += tile.getPopulation() * rate;

    // Move population that cannot be sustained by the tile into
    // the pool
    if (tile.getPopulation() > maxPop)
    {
        pool += tile.getPopulation() - maxPop;
        tile.getPopulation() = maxPop;
    }

    return tile.getPopulation();
}
