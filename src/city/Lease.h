#pragma once

class Person;
class Housing;

class Lease
{
public:
    Lease(const Person* tenant, const Housing* housing, float rent);

    const Person* getTenant() const;
    const Housing* getHousing() const;
    float getRent() const;

private:
    const Person* mTenant;
    const Housing* mHousing;
    float mRent;
};
