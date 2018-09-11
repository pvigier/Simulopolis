#pragma once

#include <ostream>
#include <boost/serialization/access.hpp>
#include <SFML/System/Vector2.hpp>
#include "common.h"

// Benchmark with and without inlining

class Vector2f
{
public:
    float x;
    float y;

    Vector2f(float x = 0.0f, float y = 0.0f);

    // Implicit conversions with SFML
    Vector2f(sf::Vector2f u);
    operator sf::Vector2f() const;


    const Vector2f& operator+=(const Vector2f& u);
    const Vector2f& operator-=(const Vector2f& u);
    const Vector2f& operator*=(float t);
    const Vector2f& operator/=(float t);
    bool operator==(const Vector2f& u) const;

    void zero();
    bool isAlmostZero() const;
    bool almostEquals(const Vector2f& u) const;

    float squaredNorm() const;
    float norm() const;
    void normalize();
    Vector2f normalized() const;
    void truncate(float maxNorm);
    Vector2f truncated(float maxNorm) const;
    float squaredDistanceTo(const Vector2f& u) const;
    float distanceTo(const Vector2f& u) const;
    float dot(const Vector2f& u) const;
    Vector2f orthogonal() const;
    float angle() const;

private:
    // Serialization
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & x & y;
    }
};

Vector2f operator*(const Vector2f& u, double t);
Vector2f operator*(double t, const Vector2f& u);
Vector2f operator-(const Vector2f& u, const Vector2f& v);
Vector2f operator+(const Vector2f& u, const Vector2f& v);
Vector2f operator/(const Vector2f& u, double t);
std::ostream& operator<<(std::ostream& os, const Vector2f& u);
