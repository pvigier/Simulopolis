#pragma once

#include <random>

class RandomGenerator
{
public:
    using result_type = uint64_t;
    using generator_type = std::mt19937_64;

    static constexpr result_type min()
    {
        return generator_type::min();
    }

    static constexpr result_type max()
    {
        return generator_type::max();
    }

    RandomGenerator(result_type seed = 0);
    ~RandomGenerator();

    result_type getSeed();
    void setSeed(result_type seed);
    result_type operator()();

private:
    result_type mSeed;
    generator_type mGenerator;
};
