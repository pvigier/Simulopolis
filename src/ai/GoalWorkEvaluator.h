#pragma once

#include "ai/GoalEvaluator.h"

class GoalWorkEvaluator : public GoalEvaluator
{
public:
    GoalWorkEvaluator(float bias);

    virtual float computeDesirability(Person* person) override;

    virtual void setGoal(Person* person) override;

private:
};
