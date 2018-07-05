#include "Housing.h"

Housing::Housing(const std::string& name, Type type, unsigned int nbStairs, unsigned int nbInhabitants, float quality) :
    Building(name, type, nbStairs), mNbInhabitants(nbInhabitants), mQuality(quality)
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
