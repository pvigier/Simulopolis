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
    static std::vector<std::unique_ptr<Work>>* getEmployees(Building* building);
    static const std::vector<std::unique_ptr<Work>>* getEmployees(const Building* building);

    Company(std::string name, int creationYear, Person* owner = nullptr);
    ~Company();

    void update(float dt);

    // Data
    const std::string& getName() const;
    const City* getCity();
    void setCity(const City* city, MessageBus* messageBus, bool alreadyAdded = false);
    MessageBus* getMessageBus();
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
    double getWholesaleMargin(Good::Type goodType) const;
    void setWholesaleMargin(Good::Type goodType, double margin);
    double getRetailMargin(Good::Type goodType) const;
    void setRetailMargin(Good::Type goodType, double margin);

private:
    // Data
    std::string mName;
    int mCreationYear;
    const City* mCity;
    MessageBus* mMessageBus;
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

    // Serialization
    friend class boost::serialization::access;

    Company() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mName & mCreationYear & mOwner & mMailbox & mAccount;
        ar & mBuildings;
        ar & mRents & mSalaries & mWholesaleMargins & mRetailMargins;
    }
};
