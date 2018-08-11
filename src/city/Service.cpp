#include "Service.h"

Service::Service(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbEmployees,
        Work::Type employeeType) :
    Building(name, type, nbStairs), mEmployees(nbEmployees, Work(employeeType, this))
{
    //ctor
}

Service::~Service()
{
    //dtor
}

std::unique_ptr<Tile> Service::clone() const
{
    return std::unique_ptr<Tile>(new Service(mTextureName, mType, mNbStairs, mEmployees.size(), mEmployees.back().getType()));
}

void Service::setOwner(Company* owner)
{
    Building::setOwner(owner);
    for (Work& employee : mEmployees)
        employee.setEmployer(mOwner);
}

std::vector<Work>& Service::getEmployees()
{
    return mEmployees;
}

const std::vector<Work>& Service::getEmployees() const
{
    return mEmployees;
}
