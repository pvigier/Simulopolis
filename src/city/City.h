#pragma once

#include <vector>
#include <memory>
#include "pcg/PersonGenerator.h"
#include "pcg/CompanyGenerator.h"
#include "city/Map.h"
#include "city/Market.h"

class GameStateEditor;
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

    City(GameStateEditor* gameStateEditor);

    void load(const std::string& name);
    void save(const std::string& name);
    void createMap(const Array2<Tile::Type>& tiles);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float dt);
    void bulldoze(Tile::Type type);

    Intersection intersect(const sf::Vector2f& position);

    void removeImmigrant(Person* person);
    void createCitizen(Person* person);

    Map& getMap();
    const Map& getMap() const;
    unsigned int getMonth() const;
    unsigned int getYear() const;
    unsigned int getPopulation() const;
    unsigned int getUnemployed() const;
    unsigned int getFunds() const;
    void decreaseFunds(unsigned int amount);
    const Person* getPerson(Id id) const;
    const std::vector<Person*>& getCitizens() const;
    const std::vector<Person*>& getImmigrants() const;
    VMarket* getMarket(VMarket::Type type);

    sf::Vector2i toTileIndices(const sf::Vector2f& position) const;
    float toHumanTime(float cityTime) const; // cityTime is expressed in hours

private:
    GameStateEditor* mGameStateEditor;

    // Generators
    PersonGenerator mPersonGenerator;
    CompanyGenerator mCompanyGenerator;

    float mCurrentTime;
    float mTimePerMonth;
    unsigned int mMonth;

    Map mMap;

    unsigned int mUnemployed;

    unsigned int mFunds;

    IdManager<std::unique_ptr<Person>> mPersons;
    std::vector<Person*> mCitizens;
    std::vector<Person*> mImmigrants;
    std::vector<std::unique_ptr<Company>> mCompanies;
    std::vector<std::unique_ptr<VMarket>> mMarkets;
    Array2<std::vector<const Car*>> mCarsByTile;

    void generateImmigrant();
    void removeCitizen(Person* person);
};

