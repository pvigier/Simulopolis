#pragma once

#include "Building.h"
#include "Good.h"

class Work;

class Industry : public Building
{
public:
    Industry(const std::string& name, Type type, unsigned int nbStairs, Good good, unsigned int productivity, std::size_t nbEmployees);
    virtual ~Industry();

    virtual std::unique_ptr<Tile> clone() const override;

protected:
    Good mGood;
    unsigned int mProductivity;
    std::size_t mNbEmployees;
    Work* mManager;
    std::vector<Work*> mEmployees;
};
