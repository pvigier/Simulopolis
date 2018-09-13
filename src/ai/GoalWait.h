#pragma once

#include "ai/Goal.h"

class GoalWait : public Goal
{
public:
    GoalWait(Person* owner, float nbHours);
    virtual ~GoalWait();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    float mDuration;
    float mEndTime;
};
