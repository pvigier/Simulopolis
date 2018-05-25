#include "Company.h"

Company::Company(std::string name) : mName(std::move(name))
{
    //ctor
}

const std::string& Company::getName() const
{
    return mName;
}

Person* Company::getManager() const
{
    return mManager;
}

void Company::setManager(Person* manager)
{
    mManager = manager;
}

const std::vector<Person*>& Company::getEmployees() const
{
    return mEmployees;
}

void Company::addEmployee(Person* employee)
{
    mEmployees.push_back(employee);
}
