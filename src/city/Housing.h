#pragma once

#include "Building.h"

class Persons;

class Housing : public Building
{
public:
    Housing(const std::string& name, Type type, unsigned int nbStairs, unsigned int nbInhabitants, float quality);
    virtual ~Housing();

    virtual std::unique_ptr<Tile> clone() const override;

protected:
    unsigned int mNbInhabitants;
    std::vector<const Person*> mInhabitants;
    float mQuality;
};
