#pragma once

#include "Building.h"

class Work;

class Service : public Building
{
public:
    Service(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbEmployees);
    virtual ~Service();

    virtual std::unique_ptr<Tile> clone() const override;

protected:
    std::size_t mNbEmployees;
    std::vector<Work*> mEmployees;
    std::vector<Person*> mCustomers;
};
