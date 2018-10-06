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
 #include <string>
 #include "message/Mailbox.h"
 #include "city/Tile.h"
 #include "city/Money.h"

class MessageBus;
class Person;
class City;
class Building;
class Lease;
enum class GoodType;
class Work;
enum class Qualification;

class Company
{
public:
    static std::vector<std::unique_ptr<Work>>* getEmployees(Building* building);
    static const std::vector<std::unique_ptr<Work>>* getEmployees(const Building* building);

    Company(std::string name, int creationYear, Person* owner, Money funds);
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
    Money getSalary(Qualification qualification) const;
    void setSalary(Qualification qualification, Money salary);
    double getWholesaleMargin(GoodType goodType) const;
    void setWholesaleMargin(GoodType goodType, double margin);
    double getRetailMargin(GoodType goodType) const;
    void setRetailMargin(GoodType goodType, double margin);

private:
    // Data
    std::string mName;
    int mCreationYear;
    const City* mCity;
    MessageBus* mMessageBus;
    const Person* mOwner;
    Mailbox mMailbox;

    // Finance
    Money mFunds;
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

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mName & mCreationYear & mOwner & mMailbox & mAccount;
        ar & mBuildings;
        ar & mRents & mSalaries & mWholesaleMargins & mRetailMargins;
    }
};
