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

#include <set>
#include "city/Building.h"

class Work;
enum class WorkType;

class Service : public Building
{
public:
    Service(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbEmployees,
        WorkType employeeType);
    virtual ~Service();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    std::vector<std::unique_ptr<Work>>& getEmployees();
    const std::vector<std::unique_ptr<Work>>& getEmployees() const;

protected:
    std::vector<std::unique_ptr<Work>> mEmployees;
    std::set<Id> mWorksInMarket;

private:
    // Serialization
    friend class boost::serialization::access;

    Service() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Building>(*this);
        ar & mEmployees & mWorksInMarket;
    }
};
