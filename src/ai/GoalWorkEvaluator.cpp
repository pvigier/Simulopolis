#include "ai/GoalWorkEvaluator.h"
#include "city/Person.h"
#include "ai/GoalWork.h"

GoalWorkEvaluator::GoalWorkEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalWorkEvaluator::computeDesirability(Person* person)
{
    if (person->getWork() && !person->getWork()->hasWorkedThisMonth())
        return mBias * 1.0f;
    return 0.0f;
}

void GoalWorkEvaluator::setGoal(Person* person)
{
    person->getShortTermBrain().pushFront(std::make_unique<GoalWork>(person));
}
