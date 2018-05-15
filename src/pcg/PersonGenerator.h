#pragma once

#include <chrono>
#include <random>
#include <fstream>
#include "city/Person.h"

class PersonGenerator
{
public:
    PersonGenerator();

    void setUp();

    Person generate(int year);

private:
    std::default_random_engine mGenerator;
    std::vector<std::string> mMaleFirstNames;
    std::vector<std::string> mFemaleFirstNames;
    std::vector<std::string> mLastNames;
};
