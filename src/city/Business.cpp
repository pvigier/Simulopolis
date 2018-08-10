#include "Business.h"

Business::Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t nbEmployees,
    Work::Type employeeType) :
    Building(name, type, nbStairs), mGood(good), mQuantity(0), mPrice(0.0f),
    mManager(Work::Type::MANAGER, this), mEmployees(nbEmployees, Work(employeeType, this))
{
    //ctor
}

Business::~Business()
{
    //dtor
}

std::unique_ptr<Tile> Business::clone() const
{
    return std::unique_ptr<Tile>(new Business(mTextureName, mType, mNbStairs, mGood, mEmployees.size(), mEmployees.front().getType()));
}

void Business::setOwner(Company* owner)
{
    Building::setOwner(owner);
    mManager.setEmployer(mOwner);
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
    return mManager;
}

std::vector<Work>& Business::getEmployees()
{
    return mEmployees;
}
