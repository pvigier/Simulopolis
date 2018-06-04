#pragma once

#include <random>
#include "city/Company.h"

class Person;

class CompanyGenerator
{
public:
    CompanyGenerator();

    void setUp();

    Company generate(int year, Person* owner);


private:
    std::default_random_engine mGenerator;
    std::vector<std::string> mFruits;
    std::vector<std::string> mPrefixes;
    std::vector<std::string> mSuffixes;
    std::vector<std::string> mDomains;
};
