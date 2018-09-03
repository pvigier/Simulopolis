 #pragma once

 #include <vector>
 #include <string>
 #include "message/Mailbox.h"
 #include "city/Tile.h"
 #include "city/Work.h"
 #include "city/Good.h"

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
    ~Company();

    void update(float dt);

    MessageBus* getMessageBus();

    // Data
    const std::string& getName() const;
    const City* getCity();
    void setCity(const City* city);
    const Person* getOwner() const;
    void setOwner(const Person* owner);
    Id getMailboxId() const;
    Id getAccount() const;
    Money getAccountBalance() const;

    // Buildings
    const std::vector<Building*>& getBuildings() const;
    void addBuilding(Building* building);
    void removeBuilding(Building* building);

    // Rent and salaries
    Money getRent(Tile::Type housingType) const;
    void setRent(Tile::Type housingType, Money rent);
    Money getSalary(Work::Qualification qualification) const;
    void setSalary(Work::Qualification qualification, Money salary);
    double getWholesaleMargin(Good good) const;
    void setWholesaleMargin(Good good, double margin);
    double getRetailMargin(Good good) const;
    void setRetailMargin(Good good, double margin);

private:
    static MessageBus* sMessageBus;

    // Data
    std::string mName;
    int mCreationYear;
    const City* mCity;
    const Person* mOwner;
    Mailbox mMailbox;
    Id mAccount;

    // Buildings
    std::vector<Building*> mBuildings;

    // Rent, salaries and prices
    std::array<Money, 3> mRents;
    std::array<Money, 3> mSalaries;
    std::array<double, 3> mWholesaleMargins;
    std::array<double, 3> mRetailMargins;

    void addToMarket(Lease* lease);
    void addToMarket(Work* work);

    // Events
    void onNewMonth();
    void onNewMinimumWage(Money minimumWage);
};
