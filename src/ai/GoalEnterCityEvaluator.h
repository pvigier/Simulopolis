#pragma once

#include "ai/GoalEvaluator.h"

class GoalEnterCityEvaluator : public GoalEvaluator
{
public:
    GoalEnterCityEvaluator(float bias);

    virtual float computeDesirability(Person* person) const override;

    virtual void setGoal(Person* person) const override;

private:
};
