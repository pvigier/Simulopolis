 #pragma once

 #include <vector>
 #include <string>
 #include "message/Mailbox.h"

class MessageBus;
class Person;
class City;
class Building;
class Lease;
class Work;

class Company
{
public:
    static void setMessageBus(MessageBus* messageBus);

    Company(std::string name, int creationYear, Person* owner = nullptr);

    const std::string& getName() const;
    void setCity(City* city);
    Person* getOwner() const;
    void setOwner(Person* owner);
    const std::vector<Building*>& getBuildings() const;
    void addBuilding(Building* building);

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

    void addToMarket(Lease& lease);
    void addToMarket(Work& work);
};
