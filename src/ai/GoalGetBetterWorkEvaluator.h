#pragma once

#include "ai/GoalEvaluator.h"

class GoalGetBetterWorkEvaluator : public GoalEvaluator
{
public:
    GoalGetBetterWorkEvaluator(float bias);

    virtual float computeDesirability(Person* person) const override;

    virtual void setGoal(Person* person) const override;

private:
};
