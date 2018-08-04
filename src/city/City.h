#pragma once

#include <vector>
#include <memory>
#include "message/Subject.h"
#include "pcg/PersonGenerator.h"
#include "pcg/CompanyGenerator.h"
#include "city/Map.h"
#include "city/Market.h"

class Building;

class City : public sf::Drawable, public Subject
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

    struct Event
    {
        enum class Type{NEW_MONTH, NEW_YEAR, NEW_IMMIGRANT};

        Type type;
        union
        {
            unsigned int month;
            unsigned int year;
            Person* person;
        };
    };

    City();

    void load(const std::string& name);
    void save(const std::string& name);
    void createMap(const Array2<Tile::Type>& tiles);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);
    void bulldoze(Tile::Type type);

    Intersection intersect(const sf::Vector2f& position);

    void eject(Person* person);
    void welcome(Person* person);

    Map& getMap();
    const Map& getMap() const;
    unsigned int getMonth() const;
    unsigned int getYear() const;
    std::string getFormattedMonth() const;
    unsigned int getPopulation() const;
    unsigned int getUnemployed() const;
    unsigned int getFunds() const;
    void decreaseFunds(unsigned int amount);
    Person* getPerson(Id id);
    const std::vector<Person*>& getCitizens() const;
    const std::vector<Person*>& getImmigrants() const;
    Building* getBuilding(Id id);
    VMarket* getMarket(VMarket::Type type);

    sf::Vector2i toTileIndices(const sf::Vector2f& position) const;
    float toHumanTime(float cityTime) const; // cityTime is expressed in hours

private:
    // Generators
    PersonGenerator mPersonGenerator;
    CompanyGenerator mCompanyGenerator;

    float mCurrentTime;
    float mTimePerMonth;
    unsigned int mMonth;
    unsigned int mYear;
    Map mMap;
    unsigned int mUnemployed;
    unsigned int mFunds;
    Company mCityCompany;

    // Agents
    IdManager<std::unique_ptr<Person>> mPersons;
    std::vector<Person*> mCitizens;
    std::vector<Person*> mImmigrants;
    std::vector<std::unique_ptr<Company>> mCompanies;
    IdManager<Building*> mBuildings;
    std::vector<std::unique_ptr<VMarket>> mMarkets;
    Array2<std::vector<const Car*>> mCarsByTile;

    void generateImmigrant();
    void removeCitizen(Person* person);

    void onNewMonth();
    void onNewYear();
};

