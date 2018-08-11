#include "Industry.h"

Industry::Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int productivity,
    std::size_t nbEmployees, Work::Type employeeType) :
    Building(name, type, nbStairs), mGood(good), mProductivity(productivity)
{
    mEmployees.push_back(Work(Work::Type::MANAGER, this));
    mEmployees.resize(nbEmployees + 1, Work(employeeType, this));
}

Industry::~Industry()
{
    //dtor
}

std::unique_ptr<Tile> Industry::clone() const
{
    return std::unique_ptr<Tile>(new Industry(mTextureName, mType, mNbStairs, mGood, mProductivity, mEmployees.size() - 1, mEmployees.back().getType()));
}

void Industry::setOwner(Company* owner)
{
    Building::setOwner(owner);
    for (Work& employee : mEmployees)
        employee.setEmployer(mOwner);
}

Work& Industry::getManager()
{
    return mEmployees[0];
}

const Work& Industry::getManager() const
{
    return mEmployees[0];
}

std::vector<Work>& Industry::getEmployees()
{
    return mEmployees;
}

const std::vector<Work>& Industry::getEmployees() const
{
    return mEmployees;
}
