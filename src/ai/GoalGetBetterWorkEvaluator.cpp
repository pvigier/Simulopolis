#include "ai/GoalGetBetterWorkEvaluator.h"
#include "city/Person.h"
#include "ai/GoalGetBetterWork.h"

GoalGetBetterWorkEvaluator::GoalGetBetterWorkEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalGetBetterWorkEvaluator::computeDesirability(Person* person) const
{
    if (person->getHome())
        return mBias * std::exp(-person->getLastMonthOutcome());
    return 0.0f;
}

void GoalGetBetterWorkEvaluator::setGoal(Person* person) const
{
    person->getLongTermBrain().pushFront(std::make_unique<GoalGetBetterWork>(person, 3));
}
