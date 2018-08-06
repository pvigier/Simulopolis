#pragma once

#include "ai/Goal.h"
#include "ai/Path.h"

class Building;

class GoalMoveTo : public Goal
{
public:
    GoalMoveTo(Person* owner, const Building* target);
    virtual ~GoalMoveTo();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    static constexpr float ARRIVE_DISTANCE = 1.0f;

    const Building* mTarget;
};
