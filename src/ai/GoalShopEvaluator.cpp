/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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

float GoalShopEvaluator::computeDesirability(Person* person)
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
        return mShopAvailable * mBias * (1.0f - person->getNeed(Person::Need::SATIETY));
    }
    return 0.0f;
}

void GoalShopEvaluator::setGoal(Person* person)
{
    person->getShortTermBrain().pushFront(std::make_unique<GoalShop>(person));
}
