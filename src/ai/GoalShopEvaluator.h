#pragma once

#include "ai/GoalEvaluator.h"

class GoalShopEvaluator : public GoalEvaluator
{
public:
    GoalShopEvaluator(float bias);

    virtual float computeDesirability(Person* person) const override;

    virtual void setGoal(Person* person) const override;

private:
};
