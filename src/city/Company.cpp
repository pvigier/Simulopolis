#include "Company.h"

Company::Company(std::string name, std::string suffix, int creationYear, Person* owner) :
    mName(std::move(name)), mSuffix(std::move(suffix)), mCreationYear(creationYear), mOwner(owner)
{
    //ctor
}

const std::string& Company::getName() const
{
    return mName;
}

std::string Company::getFullName() const
{
    return mName + " " + mSuffix;
}

Person* Company::getOwner() const
{
    return mOwner;
}

void Company::setOwner(Person* owner)
{
    mOwner = owner;
}

const std::vector<Person*>& Company::getEmployees() const
{
    return mEmployees;
}

void Company::addEmployee(Person* employee)
{
    mEmployees.push_back(employee);
}
