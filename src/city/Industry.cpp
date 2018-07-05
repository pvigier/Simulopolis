#include "Industry.h"

Industry::Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int quantity, std::size_t nbEmployees) :
    Building(name, type, nbStairs), mGood(good), mQuantity(quantity), mNbEmployees(nbEmployees)
{
    //ctor
}

Industry::~Industry()
{
    //dtor
}

std::unique_ptr<Tile> Industry::clone() const
{
    return std::unique_ptr<Tile>(new Industry(*this));
}
