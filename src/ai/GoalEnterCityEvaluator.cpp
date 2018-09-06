#include "ai/GoalEnterCityEvaluator.h"
#include "city/Person.h"
#include "ai/GoalEnterCity.h"

GoalEnterCityEvaluator::GoalEnterCityEvaluator(float bias) : GoalEvaluator(bias), mAlreadySelected(false)
{
    //ctor
}

float GoalEnterCityEvaluator::computeDesirability(Person* person)
{
    if (!mAlreadySelected)
        return mBias;
    return 0.0f;
}

void GoalEnterCityEvaluator::setGoal(Person* person)
{
    mAlreadySelected = true;
    person->getLongTermBrain().pushFront(std::make_unique<GoalEnterCity>(person));
}
