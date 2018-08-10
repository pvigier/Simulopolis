#pragma once

#include "city/Building.h"
#include "city/Lease.h"

class Housing : public Building
{
public:
    Housing(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbInhabitants, float comfort);
    virtual ~Housing();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    std::vector<Lease>& getLeases();
    float getComfort() const;

protected:
    std::vector<Lease> mLeases;
    float mComfort; // Happiness per month
};
