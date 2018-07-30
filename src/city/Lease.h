#pragma once

class Person;
class Housing;
class Company;

class Lease
{
public:
    Lease(Housing* housing);

    const Person* getTenant() const;
    void setTenant(Person* tenant);
    const Housing* getHousing() const;
    void setHousing(Housing* housing);
    const Company* getOwner() const;
    float getRent() const;
    void setRent(float rent);

private:
    Person* mTenant;
    Housing* mHousing;
    Company* mOwner;
    float mRent;
};
