#include "city/Lease.h"

Lease::Lease(const Person* tenant, const Housing* housing, float rent) :
    mTenant(tenant), mHousing(housing), mRent(rent)
{

}

const Person* Lease::getTenant() const
{
    return mTenant;
}

const Housing* Lease::getHousing() const
{
    return mHousing;
}

float Lease::getRent() const
{
    return mRent;
}
