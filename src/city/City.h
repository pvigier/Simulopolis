/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    static constexpr Money SEED_MONEY = Money(30000.0);

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
        enum class Type{NEW_MONTH, NEW_YEAR, NEW_IMMIGRANT, IMMIGRANT_EJECTED, NEW_CITIZEN, CITIZEN_LEFT,
            NEW_MINIMUM_WAGE, BUILDING_DESTROYED, REMOVE_CITIZEN};

        Type type;
        union
        {
            unsigned int month;
            unsigned int year;
            Person* person;
            Money minimumWage;
            Building* building;
        };

        Event();
        Event(Type type, unsigned int data);
        Event(Type type, Person* person);
        Event(Money minimumWage);
        Event(Building* building);
    };

    City();
    ~City();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);

    // Messaging
    void setGameMessageBus(MessageBus* messageBus);
    Id getMailboxId() const;

    // Name
    const std::string& getName() const;

    // Map
    void createMap(std::string name, uint64_t seed);
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
    MarketBase* getMarket(MarketBase::Type type);
    const MarketBase* getMarket(MarketBase::Type type) const;

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
    float getHumanTime() const;
    float getTimePerMonth() const;
    float computeNbHoursInAmonth(float nbHoursInAWeek) const;
    float toHumanTime(float cityTime) const; // cityTime is expressed in hours
    float toCityTime(float humanTime) const;

private:
    // Messaging
    MessageBus mCityMessageBus;
    Mailbox mMailbox;

    // Generators
    RandomGenerator mRandomGenerator;
    TerrainGenerator mTerrainGenerator;
    PersonGenerator mPersonGenerator;
    CompanyGenerator mCompanyGenerator;

    // Name
    std::string mName;

    // Map
    Map mMap;

    // Date
    float mTotalTime;
    float mCurrentTime;
    float mTimePerMonth;
    unsigned int mMonth;
    unsigned int mYear;

    // Economy
    Bank mBank;
    std::vector<std::unique_ptr<MarketBase>> mMarkets;
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

    // Set up
    void setUp(bool loading);

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        ar & mMailbox;
        ar & mName;
        ar & mCityCompany;
        ar & mMap;
        ar & mTotalTime & mCurrentTime & mTimePerMonth & mMonth & mYear;
        ar & mBank & mMarkets & mWorldAccount;
        ar & mWeeklyStandardWorkingHours & mMinimumWage & mIncomeTax & mCorporateTax;
        ar & mPersons & mCitizens & mImmigrants & mCompanies & mBuildings;
        ar & mTimeBeforeLeaving;
        ar & mHappiness & mAttractiveness;
        ar & mCityMessageBus;
    }

    template<typename Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        ar & mMailbox;
        ar & mName;
        ar & mCityCompany;
        ar & mMap;
        ar & mTotalTime & mCurrentTime & mTimePerMonth & mMonth & mYear;
        ar & mBank & mMarkets & mWorldAccount;
        ar & mWeeklyStandardWorkingHours & mMinimumWage & mIncomeTax & mCorporateTax;
        ar & mPersons & mCitizens & mImmigrants & mCompanies & mBuildings;
        ar & mTimeBeforeLeaving;
        ar & mHappiness & mAttractiveness;
        ar & mCityMessageBus;
        setUp(true);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

