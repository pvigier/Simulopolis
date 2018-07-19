#pragma once

#include "ai/Goal.h"

class GoalRest : public Goal
{
public:
    GoalRest(Person* owner, float duration);
    virtual ~GoalRest();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

private:
    float mDuration;
};
