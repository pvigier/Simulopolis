/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

#include <cmath>
#include <limits>
#include <algorithm>

// Types
constexpr float EPSILON = 1e-5;
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr float FLOAT_INFINITY = std::numeric_limits<float>::max();//std::numeric_limits<float>::infinity();

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
