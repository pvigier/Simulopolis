#include "Industry.h"

Industry::Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int productivity,
    std::size_t nbEmployees, Work::Type employeeType) :
    Building(name, type, nbStairs), mGood(good), mProductivity(productivity),
    mManager(Work::Type::MANAGER, this), mEmployees(nbEmployees, Work(employeeType, this))
{
    //ctor
}

Industry::~Industry()
{
    //dtor
}

std::unique_ptr<Tile> Industry::clone() const
{
    return std::unique_ptr<Tile>(new Industry(mTextureName, mType, mNbStairs, mGood, mProductivity, mEmployees.size(), mEmployees.front().getType()));
}

void Industry::setOwner(Company* owner)
{
    Building::setOwner(owner);
    mManager.setEmployer(mOwner);
    for (Work& employee : mEmployees)
        employee.setEmployer(mOwner);
}
