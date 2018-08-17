#include "ai/GoalRestEvaluator.h"
#include "city/Person.h"
#include "ai/GoalRest.h"

GoalRestEvaluator::GoalRestEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalRestEvaluator::computeDesirability(Person* person) const
{
    if (person->getHome())
        return mBias * (1.0f - person->getEnergy());
    return 0.0f;
}

void GoalRestEvaluator::setGoal(Person* person) const
{
    person->getShortTermBrain().pushFront(new GoalRest(person));
}
