#pragma once

#include "city/Building.h"
#include "city/Lease.h"

class Housing : public Building
{
public:
    static constexpr float ENERGY_GROWTH_RATE = 0.3f;

    Housing(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbInhabitants, float comfort);
    virtual ~Housing();

    virtual std::unique_ptr<Tile> clone() const override;

    virtual void setOwner(Company* owner) override;

    std::vector<Lease>& getLeases();
    const std::vector<Lease>& getLeases() const;
    float getComfort() const;

protected:
    std::vector<Lease> mLeases;
    float mComfort; // Happiness per month
};
