#include "Kinematic.h"

Kinematic::Kinematic(float mass, float maxSpeed, float maxForce, float maxRotation) :
    mMass(mass), mMaxSpeed(maxSpeed), mMaxForce(maxForce), mMaxRotation(maxRotation)
{
    //ctor
}

void Kinematic::update(float dt)
{
    Vector2f acceleration = mAccForce.truncated(mMaxForce) / mMass; // Replace mMass by mInvMass

    mVelocity += acceleration * dt;
    mVelocity.truncate(mMaxSpeed);

    mPosition += mVelocity * dt;

    if (!mVelocity.isAlmostZero())
    {
        mDirection = mVelocity.normalized();
        mSide = mDirection.orthogonal();
    }

    resetAccumulator();
}

void Kinematic::addForce(const Vector2f& force)
{
    mAccForce += force;
}

void Kinematic::resetAccumulator()
{
    mAccForce = Vector2f();
}

const Vector2f& Kinematic::getPosition() const
{
    return mPosition;
}

void Kinematic::setPosition(const Vector2f& position)
{
    mPosition = position;
}

const Vector2f& Kinematic::getDirection() const
{
    return mDirection;
}

void Kinematic::setDirection(const Vector2f& direction)
{
    mDirection = direction;
}

const Vector2f& Kinematic::getVelocity() const
{
    return mVelocity;
}

float Kinematic::getMass() const
{
    return mMass;
}

float Kinematic::getMaxSpeed() const
{
    return mMaxSpeed;
}

float Kinematic::getMaxForce() const
{
    return mMaxForce;
}
