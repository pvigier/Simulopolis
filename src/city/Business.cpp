#include "Business.h"

Business::Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t nbEmployees,
    Work::Type employeeType) :
    Building(name, type, nbStairs), mGood(good), mQuantity(0), mPrice(0.0f)
{
    mEmployees.push_back(Work(Work::Type::MANAGER, this));
    mEmployees.resize(nbEmployees + 1, Work(employeeType, this));
}

Business::~Business()
{
    //dtor
}

std::unique_ptr<Tile> Business::clone() const
{
    return std::unique_ptr<Tile>(new Business(mTextureName, mType, mNbStairs, mGood, mEmployees.size() - 1, mEmployees.back().getType()));
}

void Business::setOwner(Company* owner)
{
    Building::setOwner(owner);
    for (Work& employee : mEmployees)
        employee.setEmployer(mOwner);
}

Good Business::getGood() const
{
    return mGood;
}

float Business::getPrice() const
{
    return mPrice;
}

Work& Business::getManager()
{
    return mEmployees[0];
}

const Work& Business::getManager() const
{
    return mEmployees[0];
}

std::vector<Work>& Business::getEmployees()
{
    return mEmployees;
}

const std::vector<Work>& Business::getEmployees() const
{
    return mEmployees;
}
