#pragma once

class Person;

class GoalEvaluator
{
public:
    GoalEvaluator(float bias);
    virtual ~GoalEvaluator();

    virtual float computeDesirability(Person* person) = 0;

    virtual void setGoal(Person* person) = 0;

protected:
    float mBias;
};
