#pragma once

#include "util/Vector.h"

class Kinematic
{
public:
    Kinematic() = default;
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

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mMass & mMaxSpeed & mMaxForce & mMaxRotation;
        ar & mPosition & mDirection & mSide & mVelocity & mAccForce;
    }
};
