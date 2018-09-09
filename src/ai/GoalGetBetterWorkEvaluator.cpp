#include "ai/GoalGetBetterWorkEvaluator.h"
#include "util/common.h"
#include "city/Person.h"
#include "ai/GoalGetBetterWork.h"

GoalGetBetterWorkEvaluator::GoalGetBetterWorkEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalGetBetterWorkEvaluator::computeDesirability(Person* person)
{
    if (person->getHome())
        return mBias * sigmoid(-person->getLastMonthOutcome());
    return 0.0f;
}

void GoalGetBetterWorkEvaluator::setGoal(Person* person)
{
    person->getLongTermBrain().pushFront(std::make_unique<GoalGetBetterWork>(person, 3));
}
