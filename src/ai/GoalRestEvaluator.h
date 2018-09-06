#pragma once

#include "ai/GoalEvaluator.h"

class GoalRestEvaluator : public GoalEvaluator
{
public:
    GoalRestEvaluator(float bias);

    virtual float computeDesirability(Person* person) override;

    virtual void setGoal(Person* person) override;

private:
};
