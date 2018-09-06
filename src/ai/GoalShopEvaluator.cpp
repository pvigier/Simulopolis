#include "ai/GoalShopEvaluator.h"
#include "city/Person.h"
#include "city/City.h"
#include "city/Business.h"
#include "city/Housing.h"
#include "city/Lease.h"
#include "ai/GoalShop.h"

GoalShopEvaluator::GoalShopEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalShopEvaluator::computeDesirability(Person* person) const
{
    if (person->getHome() && person->getAccountBalance() >= 0.0)
    {
        // Find a shop
        bool mShopAvailable = false;
        Tile::Type type = Business::getBusinessType(person->getConsumptionHabit());
        std::vector<const Building*> buildings = person->getCity()->getMap().getReachableBuildingsAround(person->getHome()->getHousing(), GoalShop::RADIUS, type);
        for (const Building* building : buildings)
        {
            const Business* shop = static_cast<const Business*>(building);
            if (shop->hasPreparedGoods())
            {
                mShopAvailable = true;
                break;
            }
        }
        return mShopAvailable * mBias * (1.0f - person->getSatiety());
    }
    return 0.0f;
}

void GoalShopEvaluator::setGoal(Person* person) const
{
    person->getShortTermBrain().pushFront(std::make_unique<GoalShop>(person));
}
