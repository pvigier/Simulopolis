#pragma once

#include <vector>
#include "Map.h"

class City
{
public:
    City();
    City(std::string cityName, int tileSize, TileAtlas& tileAtlas);

    void load(std::string cityName, TileAtlas& tileAtlas);
    void save(std::string cityName);

    void update(float dt);
    void bulldoze(const Tile& tile);
    void shuffleTiles();
    void tileChanged();

    double getHomeless();
    double getUnemployed();

private:
    float mCurrentTime;
    float mTimePerDay;
    int mDay;

    std::vector<int> mShuffledTiles;

    // Number of residents who are not in a residential zone
    double mPopulationPool;
    double mPopulation;
    // Number of residents who are not currently employed but can work
    double mEmploymentPool;
    double mEmployable;
    // Proportion of citizens who can work
    float mPropCanWork;

    // Proportion of residents who die/give birth each day.
    // Estimate for death rate = 1 / (life expectancy * 360)
    // Current world values are 0.000055 and 0.000023, respectively
    double mBirthRate;
    double mDeathRate;

    Map mMap;


    double mResidentialTax;
    double mCommercialTax;
    double mIndustrialTax;

    /* Running total of city earnings (from tax etc) this month. */
    double mEarnings;
    double mFunds;

    double distributePool(double& pool, Tile& tile, double rate = 0.0);
};

