#pragma once

#include <iostream>
#include <cmath>
#include <limits>

constexpr float EPSILON = 1e-5;
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr float FLOAT_INFINITY = std::numeric_limits<float>::infinity();
constexpr float PI = M_PI;
constexpr float INV_PI = 1.0f / PI;

bool isAlmostZero(float x);
bool isSquareRootAlmostZero(float x);
bool almostEquals(float x, float y);
float toDegrees(float theta);
float toRadians(float theta);
