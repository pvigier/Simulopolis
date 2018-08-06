#pragma once

#include <iostream>
#include <cmath>
#include <limits>

// Types
constexpr float EPSILON = 1e-5;
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr float FLOAT_INFINITY = std::numeric_limits<float>::infinity();

// Constants
constexpr float PI = M_PI;
constexpr float INV_PI = 1.0f / PI;
constexpr float SQRT_2 = std::sqrt(2.0f);

bool isAlmostZero(float x);
bool isSquareRootAlmostZero(float x);
bool almostEquals(float x, float y);
float toDegrees(float theta);
float toRadians(float theta);

template<typename T>
const T& clamp(const T& v, const T& lo, const T& hi)
{
    return std::max(lo, std::min(v, hi));
}
