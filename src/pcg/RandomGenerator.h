#pragma once

#include <random>

class RandomGenerator
{
public:
    using result_type = uint64_t;
    using generator_type = std::mt19937_64;

    static result_type min();
    static result_type max();

    RandomGenerator(result_type seed = 0);
    ~RandomGenerator();

    void setSeed(result_type seed);
    result_type operator()();

private:
    result_type mSeed;
    generator_type mGenerator;
};
