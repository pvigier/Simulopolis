#pragma once

#include <string>

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
    float getRent() const;
    void setRent(float rent);

private:
    Person* mTenant;
    Housing* mHousing;
    Company* mOwner;
    float mRent;
};
