#include "city/Lease.h"

Lease::Lease(Housing* housing) :
    mTenant(nullptr), mHousing(housing), mOwner(nullptr), mRent(0.0f)
{

}

const Person* Lease::getTenant() const
{
    return mTenant;
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

float Lease::getRent() const
{
    return mRent;
}

void Lease::setRent(float rent)
{
    mRent = rent;
}
