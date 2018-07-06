#include "Service.h"

Service::Service(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbEmployees) :
    Building(name, type, nbStairs), mNbEmployees(nbEmployees)
{
    //ctor
}

Service::~Service()
{
    //dtor
}

std::unique_ptr<Tile> Service::clone() const
{
    return std::unique_ptr<Tile>(new Service(*this));
}
