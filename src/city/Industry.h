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

#include <deque>
#include <set>
#include "city/Building.h"
#include "city/Good.h"
#include "city/Work.h"

class Work;
template<typename T> class Market;

class Industry : public Building
{
public:
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good::Type goodType, double employeeProductivity,
        std::size_t nbEmployees, Work::Type employeeType);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    Good::Type getGoodType() const;
    std::unique_ptr<Work>& getManager();
    const std::unique_ptr<Work>& getManager() const;
    std::vector<std::unique_ptr<Work>>& getEmployees();
    const std::vector<std::unique_ptr<Work>>& getEmployees() const;

    // Events
    void onNewMonth();

protected:
    struct Batch
    {
        double quantity;
        Money cost;

        Money getCostPerUnit();

    private:
        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & quantity & cost;
        }
    };

    std::unique_ptr<Good> mGood;
    std::deque<Batch> mStock;
    std::set<Id> mGoodsInMarket;
    double mEmployeeProductivity; // Number of goods per month of work
    std::vector<std::unique_ptr<Work>> mEmployees;
    std::set<Id> mWorksInMarket;

    void updateStock();
    void sellGoods();
    const Market<Good>* getMarket();

private:
    // Serialization
    friend class boost::serialization::access;

    Industry() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Building>(*this);
        ar & mGood & mStock & mGoodsInMarket & mEmployeeProductivity & mEmployees & mWorksInMarket;
    }
};
