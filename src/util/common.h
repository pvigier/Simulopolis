#pragma once

#include <cmath>
#include <limits>
#include <algorithm>

// Types
constexpr float EPSILON = 1e-5;
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr float FLOAT_INFINITY = std::numeric_limits<float>::infinity();

// Constants
constexpr float PI = 3.1415926535f;
constexpr float INV_PI = 1.0f / PI;
constexpr float SQRT_2 = std::sqrt(2.0f);

bool isAlmostZero(float x);
bool isSquareRootAlmostZero(float x);
bool almostEquals(float x, float y);

constexpr float toDegrees(float theta)
{
    return 180.0f * INV_PI * theta;
}

constexpr float toRadians(float theta)
{
    return PI / 180.0f * theta;
}

template<typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return std::max(lo, std::min(v, hi));
}

float sigmoid(float t);
