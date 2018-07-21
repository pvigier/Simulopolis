#pragma once

#include "ai/GoalEvaluator.h"

class GoalRestEvaluator : public GoalEvaluator
{
public:
    GoalRestEvaluator(float bias);

    virtual float computeDesirability(Person* person) const override;

    virtual void setGoal(Person* person) const override;

private:
};
