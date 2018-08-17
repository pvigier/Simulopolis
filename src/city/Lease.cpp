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
