#pragma once

#include "Building.h"
#include "Contract.h"
#include "Good.h"

class Persons;

class Business : public Building
{
public:
    Business(const std::string& name, Type type, unsigned int nbStairs, Good good, std::size_t nbEmployees);
    virtual ~Business();

    virtual std::unique_ptr<Tile> clone() const override;

protected:
    Good mGood;
    unsigned int mQuantity;
    std::size_t mNbEmployees;
    Contract mManager;
    std::vector<Contract> mEmployees;
};
