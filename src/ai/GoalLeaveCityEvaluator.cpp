#include "ai/GoalLeaveCityEvaluator.h"
#include "city/Person.h"
#include "ai/GoalLeaveCity.h"

GoalLeaveCityEvaluator::GoalLeaveCityEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalLeaveCityEvaluator::computeDesirability(Person* person)
{
    return mBias * (1.0f - person->getNeed(Person::Need::HAPPINESS));
}

void GoalLeaveCityEvaluator::setGoal(Person* person)
{
    person->getLongTermBrain().pushFront(std::make_unique<GoalLeaveCity>(person));
}
