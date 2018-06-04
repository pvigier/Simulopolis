#include "Company.h"

Company::Company(std::string name, int creationYear, Person* owner) :
    mName(std::move(name)), mCreationYear(creationYear), mOwner(owner)
{
    //ctor
}

const std::string& Company::getName() const
{
    return mName;
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
