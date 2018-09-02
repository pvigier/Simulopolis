#include "RandomGenerator.h"

RandomGenerator::RandomGenerator(result_type seed) : mSeed(seed), mGenerator(mSeed)
{
    //ctor
}

RandomGenerator::~RandomGenerator()
{
    //dtor
}

RandomGenerator::result_type RandomGenerator::getSeed()
{
    return mSeed;
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
