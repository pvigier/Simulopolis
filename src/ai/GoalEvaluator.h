#pragma once

class Person;

class GoalEvaluator
{
public:
    GoalEvaluator(float bias);
    virtual ~GoalEvaluator();

    virtual float computeDesirability(Person* person) const = 0;

    virtual void setGoal(Person* person) const = 0;

protected:
    float mBias;
};
