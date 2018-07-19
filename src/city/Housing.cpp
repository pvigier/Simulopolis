#include "Housing.h"

Housing::Housing(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbInhabitants, float comfort) :
    Building(name, type, nbStairs), mNbInhabitants(nbInhabitants), mComfort(comfort)
{
    //ctor
}

Housing::~Housing()
{
    //dtor
}

std::unique_ptr<Tile> Housing::clone() const
{
    return std::unique_ptr<Tile>(new Housing(*this));
}

float Housing::getComfort() const
{
    return mComfort;
}
