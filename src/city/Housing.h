#pragma once

#include "Building.h"

class Persons;

class Housing : public Building
{
public:
    Housing(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbInhabitants, float comfort);
    virtual ~Housing();

    virtual std::unique_ptr<Tile> clone() const override;

    float getComfort() const;

protected:
    std::size_t mNbInhabitants;
    std::vector<const Person*> mInhabitants;
    float mComfort; // Happiness per month
};
