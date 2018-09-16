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
 
#include "SteeringBehaviors.h"
#include "Kinematic.h"

SteeringBehaviors::SteeringBehaviors(Kinematic* owner) :
    mOwner(owner), mPanicDistance(FLOAT_INFINITY), mArriveDistance(EPSILON), mSeekDistance(EPSILON)
{
    //ctor
}

SteeringBehaviors::~SteeringBehaviors()
{
    //dtor
}

Vector2f SteeringBehaviors::compute(float dt)
{
    Vector2f desiredVelocity = followPath();
    return velocityToForce(desiredVelocity, dt);
}

void SteeringBehaviors::setTarget(const Vector2f& target)
{
    mTarget = target;
}

const Path& SteeringBehaviors::getPath() const
{
    return mPath;
}

void SteeringBehaviors::setPath(Path path)
{
    mPath = std::move(path);
}

void SteeringBehaviors::setPanicDistance(float distance)
{
    mPanicDistance = distance;
}

void SteeringBehaviors::setArriveDistance(float distance)
{
    mArriveDistance = distance;
}

void SteeringBehaviors::setSeekDistance(float distance)
{
    mSeekDistance = distance;
}

Vector2f SteeringBehaviors::seek(Vector2f target) const
{
    Vector2f positionToTarget = target - mOwner->getPosition();
    float distance = positionToTarget.norm();
    if (!isAlmostZero(distance))
        return (mOwner->getMaxSpeed() / distance) * positionToTarget;
    return Vector2f(0.0f, 0.0f);
}

Vector2f SteeringBehaviors::flee(Vector2f target) const
{
    Vector2f targetToPosition = mOwner->getPosition() - target;
    float distance = targetToPosition.norm();
    if (distance < mPanicDistance)
    {
        if (isAlmostZero(distance))
            // Should return a random direction ...
            return Vector2f(1.0f, 0.0f) * mOwner->getMaxSpeed();
        else
            return (mOwner->getMaxSpeed() / distance) * targetToPosition;
    }
    return Vector2f(0.0f, 0.0f);
}

Vector2f SteeringBehaviors::arrive(Vector2f target) const
{
    Vector2f positionToTarget = target - mOwner->getPosition();
    float distance = positionToTarget.norm();
    if (!isAlmostZero(distance))
    {
        float speed = std::min(1.0f, distance / mArriveDistance) * mOwner->getMaxSpeed();
        return (speed / distance) * positionToTarget;
    }
    return Vector2f(0.0f, 0.0f);
}

Vector2f SteeringBehaviors::followPath()
{
    if (mPath.isEmpty())
        return sf::Vector2f();
    if (!mPath.isFinished())
    {
        if (mOwner->getPosition().squaredDistanceTo(mPath.getCurrentPoint()) < mSeekDistance * mSeekDistance)
            mPath.setNextPoint();
        return seek(mPath.getCurrentPoint());
    }
    else
        return arrive(mPath.getCurrentPoint());
}

Vector2f SteeringBehaviors::velocityToForce(const Vector2f& desiredVelocity, float dt) const
{
    return (desiredVelocity - mOwner->getVelocity()) * mOwner->getMass() / dt;
}
