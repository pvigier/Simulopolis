#include "common.h"

bool isAlmostZero(float x)
{
    return std::abs(x) < EPSILON;
}

bool isSquareRootAlmostZero(float x)
{
    return std::abs(x) < EPSILON;
}

bool almostEquals(float x, float y)
{
    return isAlmostZero(x - y);
}

float sigmoid(float t)
{
    return 1.0f / (1.0f + std::exp(-t));
}
