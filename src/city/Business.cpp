#include "Business.h"

Business::Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t nbEmployees) :
    Building(name, type, nbStairs), mGood(good), mQuantity(0), mNbEmployees(nbEmployees)
{
    //ctor
}

Business::~Business()
{
    //dtor
}

std::unique_ptr<Tile> Business::clone() const
{
    return std::unique_ptr<Tile>(new Business(*this));
}

Good Business::getGood() const
{
    return mGood;
}

float Business::getPrice() const
{
    return mPrice;
}
