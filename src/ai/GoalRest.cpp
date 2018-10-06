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

#include "ai/GoalRest.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Housing.h"
#include "city/Lease.h"
#include "ai/GoalMoveTo.h"
#include "ai/GoalWait.h"

GoalRest::GoalRest(Person* owner) : Goal(owner), mAtHome(false), mLastUpdate(0.0f)
{
    //ctor
}

GoalRest::~GoalRest()
{
    //dtor
}

void GoalRest::activate()
{
    mState = State::ACTIVE;

    // Add subgoals
    clearSubgoals();
    pushBack(std::make_unique<GoalMoveTo>(mOwner, mOwner->getHome()->getHousing()));
}

Goal::State GoalRest::process()
{
    activateIfInactive();

    if (!hasFailed())
    {
        mState = processSubgoals();
        if (mAtHome)
        {
            // Update sleep and happiness
            float currentTime = mOwner->getCity()->getHumanTime();
            float dmonth = (currentTime - mLastUpdate) / mOwner->getCity()->getTimePerMonth();
            mLastUpdate = currentTime;
            mOwner->increaseNeed(Person::Need::ENERGY, Housing::ENERGY_GROWTH_RATE * dmonth);
            mOwner->increaseNeed(Person::Need::HAPPINESS, mOwner->getHome()->getHousing()->getComfort() * dmonth);
        }

        // If we just arrive at home
        if (isCompleted() && !mAtHome)
        {
            mState = State::ACTIVE;
            mAtHome = true;
            mLastUpdate = mOwner->getCity()->getHumanTime();
            // Compute the number of hours needed to be rested
            float nbMonths = (1.0f - mOwner->getNeed(Person::Need::ENERGY)) / (Housing::ENERGY_GROWTH_RATE - mOwner->getDecayRate(Person::Need::ENERGY));
            nbMonths = clamp(nbMonths, 0.1f, 1.0f);
            float nbHours =  nbMonths * City::NB_HOURS_PER_MONTH;
            // Add subgoal
            pushBack(std::make_unique<GoalWait>(mOwner, nbHours));
        }
    }

    return mState;
}

void GoalRest::terminate()
{

}

bool GoalRest::handle(Message message)
{
    if (message.type == MessageType::PERSON)
    {
        const Person::Event& event = message.getInfo<Person::Event>();
        if (event.type == Person::Event::Type::EXPELLED)
        {
            mState = State::FAILED;
            return true;
        }
    }
    return false;
}

std::string GoalRest::toString() const
{
    return "Rest";
}
