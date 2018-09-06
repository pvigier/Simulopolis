#pragma once

#include "ai/GoalEvaluator.h"

class GoalEnterCityEvaluator : public GoalEvaluator
{
public:
    GoalEnterCityEvaluator(float bias);

    virtual float computeDesirability(Person* person) override;

    virtual void setGoal(Person* person) override;

private:
    bool mAlreadySelected;
};
