#pragma once

#include <vector>
#include "Map.h"
#include "Car.h"

class City : public sf::Drawable
{
public:
    City();
    City(std::string cityName);

    void load(std::string cityName);
    void save(std::string cityName);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);
    void bulldoze(Tile::Type type);

    Map& getMap();
    const Map& getMap() const;
    std::vector<Car>& getCars();
    unsigned int getDay() const;
    unsigned int getPopulation() const;
    unsigned int getUnemployed() const;
    unsigned int getFunds() const;
    void decreaseFunds(unsigned int amount);

private:
    float mCurrentTime;
    float mTimePerDay;
    unsigned int mDay;

    Map mMap;

    unsigned int mPopulation;
    unsigned int mUnemployed;

    unsigned int mFunds;

    std::vector<Car> mCars;
};

