#pragma once

#include <vector>
#include <memory>
#include "message/Subject.h"
#include "pcg/TerrainGenerator.h"
#include "pcg/PersonGenerator.h"
#include "pcg/CompanyGenerator.h"
#include "city/Map.h"
#include "city/Bank.h"
#include "city/Market.h"

class Building;

class City : public sf::Drawable, public Subject
{
public:
    static constexpr float NB_HOURS_PER_MONTH = 30.0f * 24.0f;
    static constexpr float MAX_NB_IMMIGRANTS_PER_MONTH = 10.0f;
    static constexpr float MAX_NB_MONTHS_WAITING = 3.0f;

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
        enum class Type{NEW_MONTH, NEW_YEAR, NEW_IMMIGRANT, IMMIGRANT_EJECTED, NEW_CITIZEN, NEW_MINIMUM_WAGE,
            BUILDING_DESTROYED};

        Type type;
        union
        {
            unsigned int month;
            unsigned int year;
            Person* person;
            Money minimumWage;
            Building* building;
        };

        Event(Type type, unsigned int data);
        Event(Type type, Person* person);
        Event(Money minimumWage);
        Event(Building* building);
    };

    City();

    void load(const std::string& name);
    void save(const std::string& name);
    void createMap(uint64_t seed);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Map
    void update(float dt);
    void bulldoze(Tile::Type type);
    Intersection intersect(const sf::Vector2f& position);
    Map& getMap();
    const Map& getMap() const;

    // Date
    unsigned int getMonth() const;
    unsigned int getYear() const;
    std::string getFormattedMonth() const;

    // Economy
    const Bank& getBank() const;
    VMarket* getMarket(VMarket::Type type);
    const VMarket* getMarket(VMarket::Type type) const;

    // Company
    Company& getCompany();
    Money getFunds() const;
    void decreaseFunds(Money amount);

    // Policy
    unsigned int getWeeklyStandardWorkingHours() const;
    void setWeeklyStandardWorkingHours(unsigned int weeklyStandardWorkingHours);
    Money getMinimumWage() const;
    void setMinimumWage(Money minimumWage);
    double getIncomeTax() const;
    void setIncomeTax(double incomeTax);
    double getCorporateTax() const;
    void setCorporateTax(double corporateTax);

    // Agents
    void eject(Person* person);
    void welcome(Person* person);
    unsigned int getPopulation() const;
    Person* getPerson(Id id);
    const std::vector<Person*>& getCitizens() const;
    const std::vector<Person*>& getImmigrants() const;
    Building* getBuilding(Id id);

    // Statistics
    float getAverageHappiness() const;
    float getAttractiveness() const;

    // Util
    sf::Vector2i toTileIndices(const sf::Vector2f& position) const;
    float getTimePerMonth() const;
    float computeNbHoursInAmonth(float nbHoursInAWeek) const;
    float toHumanTime(float cityTime) const; // cityTime is expressed in hours
    float toCityTime(float humanTime) const;

private:
    // Generators
    RandomGenerator mRandomGenerator;
    TerrainGenerator mTerrainGenerator;
    PersonGenerator mPersonGenerator;
    CompanyGenerator mCompanyGenerator;

    // Map
    Map mMap;

    // Date
    float mCurrentTime;
    float mTimePerMonth;
    unsigned int mMonth;
    unsigned int mYear;

    // Economy
    Bank mBank;
    std::vector<std::unique_ptr<VMarket>> mMarkets;
    Id mWorldAccount;

    // City company
    Company mCityCompany;

    // Policy
    unsigned int mWeeklyStandardWorkingHours;
    Money mMinimumWage;
    double mIncomeTax;
    double mCorporateTax;

    // Agents
    IdManager<std::unique_ptr<Person>> mPersons;
    std::vector<Person*> mCitizens;
    std::vector<Person*> mImmigrants;
    std::vector<std::unique_ptr<Company>> mCompanies;
    IdManager<Building*> mBuildings;
    Array2<std::vector<const Car*>> mCarsByTile;

    // Immigration
    std::vector<unsigned int> mTimeBeforeLeaving;

    // Statistics
    float mHappiness;
    float mAttractiveness;

    void updateImmigrants();
    void generateImmigrant();
    void removeCitizen(Person* person);

    // Statistics
    void updateStatistics();
    void computeHappiness();
    void computeAttractiveness();

    // Events
    void onNewMonth();
    void onNewYear();
};

