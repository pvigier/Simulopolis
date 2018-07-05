#pragma once

#include "Building.h"
#include "Contract.h"
#include "Good.h"

class Persons;

class Industry : public Building
{
public:
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int quantity, std::size_t nbEmployees);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;

protected:
    Good mGood;
    unsigned int mQuantity;
    std::size_t mNbEmployees;
    std::vector<Contract> mEmployees;
};
