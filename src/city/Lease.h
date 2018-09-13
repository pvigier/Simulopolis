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

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mTenant & mHousing & mOwner & mRent;
    }
};
