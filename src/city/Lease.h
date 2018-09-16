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

#include <string>
#include <boost/serialization/access.hpp>
#include "city/Money.h"

class Person;
class Housing;
class Company;

class Lease
{
public:
    Lease(Housing* housing = nullptr);

    const Person* getTenant() const;
    std::string getTenantName() const;
    void setTenant(Person* tenant);
    const Housing* getHousing() const;
    void setHousing(Housing* housing);
    const Company* getOwner() const;
    void setOwner(Company* owner);
    Money getRent() const;
    void setRent(Money rent);

private:
    Person* mTenant;
    Housing* mHousing;
    Company* mOwner;
    Money mRent;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mTenant & mHousing & mOwner & mRent;
    }
};
