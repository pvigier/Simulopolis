#include "Housing.h"

Housing::Housing(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbInhabitants, float comfort) :
    Building(name, type, nbStairs), mLeases(nbInhabitants, Lease(this)), mComfort(comfort)
{
    //ctor
}

Housing::~Housing()
{
    //dtor
}

std::unique_ptr<Tile> Housing::clone() const
{
    return std::unique_ptr<Tile>(new Housing(mTextureName, mType, mNbStairs, mLeases.size(), mComfort));
}

void Housing::setOwner(Company* owner)
{
    Building::setOwner(owner);
    for (Lease& lease : mLeases)
        lease.setOwner(mOwner);
}

std::vector<Lease>& Housing::getLeases()
{
    return mLeases;
}

float Housing::getComfort() const
{
    return mComfort;
}
