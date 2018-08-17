#pragma once

#include <string>
#include "city/Money.h"

class Person;
class Housing;
class Company;

class Lease
{
public:
    Lease(Housing* housing);

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
};
