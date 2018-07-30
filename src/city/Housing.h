#pragma once

#include "city/Building.h"
#include "city/Lease.h"

class Housing : public Building
{
public:
    Housing(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbInhabitants, float comfort);
    virtual ~Housing();

    virtual std::unique_ptr<Tile> clone() const override;

    float getComfort() const;

protected:
    std::size_t mNbInhabitants;
    std::vector<Lease> mInhabitants;
    float mComfort; // Happiness per month
};
