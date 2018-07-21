#include "ai/GoalWorkEvaluator.h"
#include "city/Person.h"
#include "ai/GoalWork.h"

GoalWorkEvaluator::GoalWorkEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalWorkEvaluator::computeDesirability(Person* person) const
{
    if (person->getWork() && !person->getWork()->hasAlreadyWorkedThisMonth())
        return mBias * 1.0f;
    return 0.0f;
}

void GoalWorkEvaluator::setGoal(Person* person) const
{
    person->getShortTermBrain().pushFront(new GoalWork(person));
}
