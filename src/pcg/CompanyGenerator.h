#pragma once

#include "pcg/RandomGenerator.h"
#include "city/Company.h"

class Person;

class CompanyGenerator
{
public:
    CompanyGenerator(RandomGenerator& generator);

    void setUp();

    Company generate(int year, Person* owner);


private:
    RandomGenerator& mGenerator;
    std::vector<std::string> mFruits;
    std::vector<std::string> mPrefixes;
    std::vector<std::string> mSuffixes;
    std::vector<std::string> mDomains;
};
