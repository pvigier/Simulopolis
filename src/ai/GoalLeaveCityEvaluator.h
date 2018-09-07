#pragma once

#include "ai/GoalEvaluator.h"

class GoalLeaveCityEvaluator : public GoalEvaluator
{
public:
    GoalLeaveCityEvaluator(float bias);

    virtual float computeDesirability(Person* person) override;

    virtual void setGoal(Person* person) override;

private:
};
