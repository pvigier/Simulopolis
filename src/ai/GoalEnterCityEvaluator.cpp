#include "ai/GoalEnterCityEvaluator.h"
#include "city/Person.h"
#include "ai/GoalEnterCity.h"

GoalEnterCityEvaluator::GoalEnterCityEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalEnterCityEvaluator::computeDesirability(Person* person) const
{
    if (!person->getHome())
        return mBias;
    return 0.0f;
}

void GoalEnterCityEvaluator::setGoal(Person* person) const
{
    person->getLongTermBrain().pushFront(new GoalEnterCity(person));
}
