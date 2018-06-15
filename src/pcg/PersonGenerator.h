#pragma once

#include <random>
#include "city/Person.h"

class PersonGenerator
{
public:
    PersonGenerator();

    void setUp();

    std::unique_ptr<Person> generate(int year);

private:
    std::default_random_engine mGenerator;
    std::vector<std::string> mMaleFirstNames;
    std::vector<std::string> mFemaleFirstNames;
    std::vector<std::string> mLastNames;
    std::vector<std::string> mCars;
};
