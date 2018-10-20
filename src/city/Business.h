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

#include <queue>
#include <set>
#include "city/Building.h"
#include "city/Money.h"

class Work;
enum class WorkType;
class Good;
enum class GoodType;

template<typename T> class Market;

class Business : public Building
{
public:
    struct Event
    {
        enum class Type{RESERVATION, RESERVATION_ACCEPTED, RESERVATION_REFUSED};

        Type type;
        Id accountId;
        Money price;
    };

    static Type getBusinessType(GoodType goodType);

    Business(const std::string& name, Type type, unsigned int nbStairs, GoodType goodType, unsigned int maxSizeStock,
        double employeeProductivity, std::size_t nbEmployees, WorkType employeeType);

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    GoodType getGoodType() const;
    unsigned int getStock() const;
    unsigned int getMaxSizeStock() const;
    bool hasPreparedGoods() const;
    unsigned int getPreparedGoods() const;
    Money getPrice() const;
    std::unique_ptr<Work>& getManager();
    const std::unique_ptr<Work>& getManager() const;
    std::vector<std::unique_ptr<Work>>& getEmployees();
    const std::vector<std::unique_ptr<Work>>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    GoodType mGoodType;
    unsigned int mMaxSizeStock;
    unsigned int mStock;
    Money mStockCost;
    double mPreparedGoods;
    Money mPrice;
    double mEmployeeProductivity; // Number of goods per month of work
    std::vector<std::unique_ptr<Work>> mEmployees;
    std::set<Id> mWorksInMarket;

    void prepareGoods();
    void buyGoods();
    const Market<Good>* getMarket();
    void updatePrice();
    void updateDesiredQuantity();

private:
    // Serialization
    friend class boost::serialization::access;

    Business() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Building>(*this);
        ar & mGoodType & mMaxSizeStock & mStock & mStockCost & mPreparedGoods & mPrice;
        ar & mEmployeeProductivity & mEmployees & mWorksInMarket;
    }
};
