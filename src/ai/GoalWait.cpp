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
 
#include "GoalWait.h"
#include "city/City.h"
#include "city/Person.h"

GoalWait::GoalWait(Person* owner, float nbHours) :
    Goal(owner), mDuration(mOwner->getCity()->toHumanTime(nbHours)), mEndTime(0.0f)
{
    //ctor
}

GoalWait::~GoalWait()
{
    //dtor
}

void GoalWait::activate()
{
    mState = State::ACTIVE;

    // Update the state of the Owner
    mOwner->setState(Person::State::WAITING);

    // Compute the end time
    mEndTime = mOwner->getCity()->getHumanTime() + mDuration;
}

Goal::State GoalWait::process()
{
    activateIfInactive();

    if (mOwner->getCity()->getHumanTime() >= mEndTime)
        mState = State::COMPLETED;

    return mState;
}

void GoalWait::terminate()
{

}

std::string GoalWait::toString() const
{
    return "Wait";
}
