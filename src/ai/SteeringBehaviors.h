#pragma once

#include "Kinematic.h"
#include "Path.h"

class SteeringBehaviors
{
public:
    SteeringBehaviors(Kinematic& owner);
    ~SteeringBehaviors();

    Vector2f compute(float dt);

    void setTarget(const Vector2f& target);
    void setPath(Path path);
    void setPanicDistance(float distance);
    void setArriveDistance(float distance);
    void setSeekDistance(float distance);

private:
    Kinematic& mOwner;
    Vector2f mTarget;
    Path mPath;
    float mPanicDistance;
    float mArriveDistance;
    float mSeekDistance;

    Vector2f seek(Vector2f target) const;
    Vector2f flee(Vector2f target) const;
    Vector2f arrive(Vector2f target) const;
    Vector2f followPath();

    inline Vector2f velocityToForce(const Vector2f& desiredVelocity, float dt) const;
};
