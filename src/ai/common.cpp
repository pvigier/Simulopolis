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

float toDegrees(float theta)
{
    return 180.0f * INV_PI * theta;
}

float toRadians(float theta)
{
    return PI / 180.0f * theta;
}
