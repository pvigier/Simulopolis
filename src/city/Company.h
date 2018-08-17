 #pragma once

 #include <vector>
 #include <string>
 #include "message/Mailbox.h"
 #include "city/Tile.h"
 #include "city/Work.h"

class MessageBus;
class Person;
class City;
class Building;
class Lease;

class Company
{
public:
    static void setMessageBus(MessageBus* messageBus);
    static std::vector<Work>* getEmployees(Building* building);
    static const std::vector<Work>* getEmployees(const Building* building);

    Company(std::string name, int creationYear, Person* owner = nullptr);

    void update(float dt);

    const std::string& getName() const;
    void setCity(City* city);
    Person* getOwner() const;
    void setOwner(Person* owner);
    Id getMailboxId() const;
    const std::vector<Building*>& getBuildings() const;
    void addBuilding(Building* building);

    Money getRent(Tile::Type housingType);
    void setRent(Tile::Type housingType, Money rent);
    Money getSalary(Work::Qualification qualification);
    void setSalary(Work::Qualification qualification, Money salary);

private:
    static MessageBus* sMessageBus;

    // Data
    std::string mName;
    int mCreationYear;
    City* mCity;
    Person* mOwner;
    Mailbox mMailbox;
    Money mFunds;

    // Buildings
    std::vector<Building*> mBuildings;

    // Rent and salaries
    std::array<Money, 3> mRents;
    std::array<Money, 3> mSalaries;

    void addToMarket(Lease& lease);
    void addToMarket(Work& work);
};
