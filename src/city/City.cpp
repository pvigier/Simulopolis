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

    mMap.load(cityName + "_map.dat", width, height);
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

void City::update(float dt)
{
    /*double popTotal = 0;
    double commercialRevenue = 0;
    double industrialRevenue = 0;

    // Update the game time
    mCurrentTime += dt;
    while (mCurrentTime >= mTimePerDay)
    {
        ++mDay;
        mCurrentTime -= mTimePerDay;
        if (mDay % 30 == 0)
        {
            mFunds += mEarnings;
            mEarnings = 0;
        }
        // Run first pass of tile updates. Mostly handles pool distribution
        for (unsigned int i = 0; i < mMap.getNbTiles(); ++i)
        {
            Tile& tile = mMap.getTile(mShuffledTiles[i]);
            TileType type = tile.getType();

            if (type == TileType::RESIDENTIAL)
            {
                // Redistribute the pool and increase the population total by the tile's population
                distributePool(mPopulationPool, tile, mBirthRate - mDeathRate);
                popTotal += tile.getPopulation();
            }
            else if (type == TileType::COMMERCIAL)
            {
                // Hire people
                if(rand() % 100 < 15 * (1.0 - mCommercialTax))
                    distributePool(mEmploymentPool, tile, 0.00);
            }
            else if (type == TileType::INDUSTRIAL)
            {
                // Extract resources from the ground
                if (mMap.getResource(mShuffledTiles[i]) > 0 && rand() % 100 < mPopulation)
                {
                    ++tile.getProduction();
                    mMap.setResource(mShuffledTiles[i], mMap.getResource(mShuffledTiles[i]) - 1);
                }
                // Hire people
                if (rand() % 100 < 15 * (1.0 - mIndustrialTax))
                    distributePool(mEmploymentPool, tile, 0.0);
            }

            tile.update();
        }

        // Run second pass. Mostly handles goods manufacture
        for (unsigned int i = 0; i < mMap.getNbTiles(); ++i)
        {
            Tile& tile = mMap.getTile(mShuffledTiles[i]);
            TileType type = tile.getType();

            if (type == TileType::INDUSTRIAL)
            {
                unsigned int receivedResources = 0;
                // Receive resources from smaller and connected zones
                for(unsigned int j = 0; j < mMap.getNbTiles(); ++j)
                {
                    Tile& otherTile = mMap.getTile(j);
                    if (otherTile.getRegions()[0] == tile.getRegions()[0] &&
                        otherTile.getType() == TileType::INDUSTRIAL)
                    {
                        if (otherTile.getProduction() > 0)
                        {
                            ++receivedResources;
                            --otherTile.getProduction();
                        }
                        if (receivedResources >= tile.getVariant() + 1)
                            break;
                    }
                }
                // Turn resources into goods
                tile.getStoredGoods() += (receivedResources + tile.getProduction()) * (tile.getVariant() + 1);
            }
        }

        // Run third pass. Mostly handles goods distribution.
        for (unsigned int i = 0; i < mMap.getNbTiles(); ++i)
        {
            Tile& tile = mMap.getTile(mShuffledTiles[i]);
            TileType type = tile.getType();

            if (type == TileType::COMMERCIAL)
            {
                unsigned int receivedGoods = 0;
                double maxCustomers = 0.0;
                for (unsigned int j = 0; j < mMap.getNbTiles(); ++j)
                {
                    Tile& otherTile = mMap.getTile(j);
                    if (otherTile.getRegions()[0] == tile.getRegions()[0] &&
                        otherTile.getType() == TileType::INDUSTRIAL &&
                        otherTile.getStoredGoods() > 0)
                    {
                        while (otherTile.getStoredGoods() > 0 && receivedGoods != tile.getVariant() + 1)
                        {
                            --otherTile.getStoredGoods();
                            ++receivedGoods;
                            industrialRevenue += 100 * (1.0 - mIndustrialTax);
                        }
                    }
                    else if (otherTile.getRegions()[0] == tile.getRegions()[0] &&
                        otherTile.getType() == TileType::RESIDENTIAL)
                        maxCustomers += otherTile.getPopulation();
                    if (receivedGoods == tile.getVariant())
                        break;
                }
                // Calculate the overall revenue for the tile
                tile.getProduction() = (receivedGoods * 100.0 + rand() % 20) * (1.0 - mCommercialTax);

                double revenue = tile.getPopulation() * maxCustomers * tile.getPopulation() / 100.0;
                commercialRevenue += revenue;
            }
        }

        // Adjust population pool for births and deaths
        mPopulationPool += mPopulationPool * (mBirthRate - mDeathRate);
        popTotal += mPopulationPool;

        // Adjust the employment pool for the changing population
        float newWorkers = (popTotal - mPopulation) * mPropCanWork;
        newWorkers *= newWorkers < 0 ? -1 : 1;
        mEmploymentPool += newWorkers;
        mEmployable += newWorkers;
        if (mEmploymentPool < 0)
            mEmploymentPool = 0;
        if (mEmployable < 0)
            mEmployable = 0;

        // Update the city population
        mPopulation = popTotal;
        std::cout << mPopulation << std::endl;

        // Calculate city income from tax
        mEarnings = (mPopulation - mPopulationPool) * 15 * mResidentialTax;
        mEarnings += commercialRevenue * mCommercialTax;
        mEarnings += industrialRevenue * mIndustrialTax;

    }*/
}

void City::bulldoze(Tile::Type type)
{
    // Replace the selected tiles on the map with the tile and
    // update populations etc accordingly
    for (unsigned int pos = 0; pos < mMap.getNbTiles(); ++pos)
    {
        if (mMap.getTileState(pos) == Tile::State::SELECTED)
        {
            /*const Tile& tileToDelete = mMap.getTile(pos);
            Tile::Type type = tileToDelete.getType();
            if (type == Tile::Type::RESIDENTIAL)
                mPopulationPool += tileToDelete.getPopulation();
            else if (type == Tile::Type::COMMERCIAL || type == Tile::Type::INDUSTRIAL)
                mEmploymentPool += tileToDelete.getPopulation();*/
            mMap.setTile(pos, Map::createTile(type));
        }
    }
}

void City::shuffleTiles()
{
    mShuffledTiles.resize(mMap.getNbTiles());
    std::iota(mShuffledTiles.begin(), mShuffledTiles.end(), 1);
    std::random_shuffle(mShuffledTiles.begin(), mShuffledTiles.end());
}

void City::tileChanged()
{
    //mMap.updateDirection(Tile::Type::ROAD);
    /*mMap.findConnectedRegions(
        {Tile::Type::ROAD, Tile::Type::RESIDENTIAL, Tile::Type::COMMERCIAL, Tile::Type::INDUSTRIAL}, 0);*/
}

Map& City::getMap()
{
    return mMap;
}

const Map& City::getMap() const
{
    return mMap;
}

int City::getDay() const
{
    return mDay;
}

double City::getPopulation() const
{
    return mPopulation;
}

double City::getEmployable() const
{
    return mEmployable;
}

double& City::getFunds()
{
    return mFunds;
}

double City::getHomeless() const
{
    return mPopulationPool;
}

double City::getUnemployed() const
{
    return mEmploymentPool;
}

/*double City::distributePool(double& pool, Tile& tile, double rate)
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
}*/
