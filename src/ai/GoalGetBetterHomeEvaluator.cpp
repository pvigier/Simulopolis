#include "ai/GoalGetBetterHomeEvaluator.h"
#include "city/Person.h"
#include "ai/GoalGetBetterHome.h"

GoalGetBetterHomeEvaluator::GoalGetBetterHomeEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalGetBetterHomeEvaluator::computeDesirability(Person* person)
{
    return mBias * (1 - person->getAverageNeed(Person::Need::ENERGY));
}

void GoalGetBetterHomeEvaluator::setGoal(Person* person)
{
    person->getLongTermBrain().pushFront(std::make_unique<GoalGetBetterHome>(person, 3));
}
