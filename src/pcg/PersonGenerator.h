#pragma once

#include "pcg/RandomGenerator.h"
#include "city/Person.h"

class PersonGenerator
{
public:
    PersonGenerator(RandomGenerator& generator);

    void setUp();

    std::unique_ptr<Person> generate(int year);

private:
    RandomGenerator& mGenerator;
    std::vector<std::string> mMaleFirstNames;
    std::vector<std::string> mFemaleFirstNames;
    std::vector<std::string> mLastNames;
    std::vector<std::string> mCars;
};
