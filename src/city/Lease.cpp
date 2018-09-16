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
 
#include "city/Lease.h"
#include "city/Person.h"

Lease::Lease(Housing* housing) :
    mTenant(nullptr), mHousing(housing), mOwner(nullptr), mRent(0.0)
{

}

const Person* Lease::getTenant() const
{
    return mTenant;
}

std::string Lease::getTenantName() const
{
    if (mTenant)
        return mTenant->getFullName();
    else
        return "Vacant";
}

void Lease::setTenant(Person* tenant)
{
    mTenant = tenant;
}

const Housing* Lease::getHousing() const
{
    return mHousing;
}

void Lease::setHousing(Housing* housing)
{
    mHousing = housing;
}

const Company* Lease::getOwner() const
{
    return mOwner;
}

void Lease::setOwner(Company* owner)
{
    mOwner = owner;
}

Money Lease::getRent() const
{
    return mRent;
}

void Lease::setRent(Money rent)
{
    mRent = rent;
}
