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

    Company(std::string name, int creationYear, Person* owner = nullptr);

    void update(float dt);

    const std::string& getName() const;
    void setCity(City* city);
    Person* getOwner() const;
    void setOwner(Person* owner);
    Id getMailboxId() const;
    const std::vector<Building*>& getBuildings() const;
    void addBuilding(Building* building);

    float getRent(Tile::Type housingType);
    void setRent(Tile::Type housingType, float rent);
    float getSalary(Work::Qualification qualification);
    void setSalary(Work::Qualification qualification, float salary);

private:
    static MessageBus* sMessageBus;

    // Data
    std::string mName;
    int mCreationYear;
    City* mCity;
    Person* mOwner;
    Mailbox mMailbox;

    // Buildings
    std::vector<Building*> mBuildings;

    // Rent and salaries
    std::array<float, 3> mRents;
    std::array<float, 3> mSalaries;

    void addToMarket(Lease& lease);
    void addToMarket(Work& work);
};
