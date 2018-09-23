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

#include "util/Vector.h"

class Kinematic
{
public:
    Kinematic() = default; // Only for serialization
    Kinematic(float mass, float maxSpeed,
        float maxForce = FLOAT_INFINITY, float maxRotation = FLOAT_INFINITY);

    void update(float dt);

    void addForce(const Vector2f& force);
    void resetAccumulator();

    const Vector2f& getPosition() const;
    void setPosition(const Vector2f& position);
    const Vector2f& getDirection() const;
    void setDirection(const Vector2f& direction);
    const Vector2f& getVelocity() const;
    float getMass() const;
    float getMaxSpeed() const;
    float getMaxForce() const;

private:
    // Characteristics
    float mMass;
    float mMaxSpeed;
    float mMaxForce;
    float mMaxRotation;
    // State
    Vector2f mPosition;
    Vector2f mDirection;
    Vector2f mSide;
    Vector2f mVelocity;
    Vector2f mAccForce;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mMass & mMaxSpeed & mMaxForce & mMaxRotation;
        ar & mPosition & mDirection & mSide & mVelocity & mAccForce;
    }
};
