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
