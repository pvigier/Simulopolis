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
 
#include "Vector.h"

Vector2f::Vector2f(float x, float y) : x(x), y(y)
{

}

Vector2f::Vector2f(sf::Vector2f u) : x(u.x), y(u.y)
{

}

Vector2f::operator sf::Vector2f() const
{
    return sf::Vector2f(x, y);
}


const Vector2f& Vector2f::operator+=(const Vector2f& u)
{
    x += u.x;
    y += u.y;
    return *this;
}

const Vector2f& Vector2f::operator-=(const Vector2f& u)
{
    x -= u.x;
    y -= u.y;
    return *this;
}

const Vector2f& Vector2f::operator*=(float t)
{
    x *= t;
    y *= t;
    return *this;
}

const Vector2f& Vector2f::operator/=(float t)
{
    x *= t;
    y *= t;
    return *this;
}

bool Vector2f::operator==(const Vector2f& u) const
{
    return (x == u.x) && (y == u.y);
}

void Vector2f::zero()
{
    x = 0;
    y = 0;
}

bool Vector2f::isAlmostZero() const
{
    return isSquareRootAlmostZero(squaredNorm());
}

bool Vector2f::almostEquals(const Vector2f& u) const
{
    return isSquareRootAlmostZero(squaredDistanceTo(u));
}

float Vector2f::squaredNorm() const
{
    return x * x + y * y;
}

float Vector2f::norm() const
{
    return std::sqrt(squaredNorm());
}

void Vector2f::normalize()
{
    *this /= norm();
}

Vector2f Vector2f::normalized() const
{
    return *this / norm();
}

void Vector2f::truncate(float maxNorm)
{
    float N = norm();
    if (N > maxNorm)
        *this *= maxNorm / N;
}

Vector2f Vector2f::truncated(float maxNorm) const
{
    float N = norm();
    if (N > maxNorm)
        return (maxNorm / N) * (*this);
    return *this;
}

float Vector2f::squaredDistanceTo(const Vector2f& u) const
{
    return (*this - u).squaredNorm();
}

float Vector2f::distanceTo(const Vector2f& u) const
{
    return std::sqrt(squaredDistanceTo(u));
}

float Vector2f::dot(const Vector2f& u) const
{
    return x * u.x + y * u.y;
}

Vector2f Vector2f::orthogonal() const
{
    return Vector2f(-y, x);
}

float Vector2f::angle() const
{
    return std::atan2(y, x);
}

Vector2f operator+(const Vector2f& u, const Vector2f& v)
{
    return Vector2f(u.x + v.x, u.y + v.y);
}

Vector2f operator-(const Vector2f& u, const Vector2f& v)
{
    return Vector2f(u.x - v.x, u.y - v.y);
}

Vector2f operator*(double t, const Vector2f& u)
{
    return Vector2f(t * u.x, t * u.y);
}

Vector2f operator*(const Vector2f& u, double t)
{
    return Vector2f(t * u.x, t * u.y);
}

Vector2f operator/(const Vector2f& u, double t)
{
    return Vector2f(u.x / t, u.y / t);
}

std::ostream& operator<<(std::ostream& os, const Vector2f& u)
{
    os << "(" << u.x << ", " << u.y << ")";
    return os;
}
