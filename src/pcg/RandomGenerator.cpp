#include "RandomGenerator.h"

RandomGenerator::result_type RandomGenerator::min()
{
    return generator_type::min();
}

RandomGenerator::result_type RandomGenerator::max()
{
    return generator_type::max();
}

RandomGenerator::RandomGenerator(result_type seed) : mSeed(seed), mGenerator(mSeed)
{
    //ctor
}

RandomGenerator::~RandomGenerator()
{
    //dtor
}

void RandomGenerator::setSeed(result_type seed)
{
    mSeed = seed;
    mGenerator.seed(0);
}

RandomGenerator::result_type RandomGenerator::operator()()
{
    return mGenerator();
}
