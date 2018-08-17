#include "ai/GoalShopEvaluator.h"
#include "city/Person.h"
#include "ai/GoalShop.h"

GoalShopEvaluator::GoalShopEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalShopEvaluator::computeDesirability(Person* person) const
{
    if (person->getFavoriteShop())
        return mBias * (1.0f - person->getSatiety());
    return 0.0f;
}

void GoalShopEvaluator::setGoal(Person* person) const
{
    person->getShortTermBrain().pushFront(new GoalShop(person));
}
