#pragma once

#include <vector>
#include <memory>
#include "Map.h"
#include "Car.h"
#include "Person.h"
#include "Company.h"

class Building;

class City : public sf::Drawable
{
public:
    struct Intersection
    {
        enum class Type{NONE, CAR, BUILDING};

        Type type;
        union
        {
            const Car* car;
            const Building* building;
        };

        Intersection();
        Intersection(const Car* car);
        Intersection(const Building* building);
    };

    City();

    void load(const std::string& name);
    void save(const std::string& name);
    void createMap(const Array2<Tile::Type>& tiles);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);
    void bulldoze(Tile::Type type);

    Intersection intersect(const sf::Vector2f& position);

    Map& getMap();
    const Map& getMap() const;
    unsigned int getDay() const;
    unsigned int getYear() const;
    unsigned int getPopulation() const;
    unsigned int getUnemployed() const;
    unsigned int getFunds() const;
    void decreaseFunds(unsigned int amount);

    sf::Vector2i toTileIndices(const sf::Vector2f& position) const;

private:
    float mCurrentTime;
    float mTimePerDay;
    unsigned int mDay;

    Map mMap;

    unsigned int mPopulation;
    unsigned int mUnemployed;

    unsigned int mFunds;

    std::vector<std::unique_ptr<Person>> mPersons;
    std::vector<std::unique_ptr<Company>> mCompanies;
    std::vector<Car> mCars;
    Array2<std::vector<const Car*>> mCarsByTile;
};

