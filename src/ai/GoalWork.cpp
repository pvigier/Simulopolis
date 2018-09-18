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
 
#include "GoalWork.h"
#include "city/Person.h"
#include "city/Work.h"
#include "city/Building.h"
#include "city/City.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalWork::GoalWork(Person* owner) : Goal(owner)
{
    //ctor
}

GoalWork::~GoalWork()
{
    //dtor
}

void GoalWork::activate()
{
    mState = State::ACTIVE;
    // Add subgoals
    clearSubgoals();
    const Work* work = mOwner->getWork();
    pushBack(std::make_unique<GoalMoveTo>(mOwner, work->getWorkplace()));
    pushBack(std::make_unique<GoalWait>(mOwner, mOwner->getCity()->computeNbHoursInAmonth(mOwner->getCity()->getWeeklyStandardWorkingHours())));
}

Goal::State GoalWork::process()
{
    activateIfInactive();

    if (!hasFailed())
        mState = processSubgoals();

    return mState;
}

void GoalWork::terminate()
{
    mOwner->getWork()->setWorkedThisMonth(true);
}

bool GoalWork::handle(Message message)
{
    if (message.type == MessageType::PERSON)
    {
        const Person::Event& event = message.getInfo<Person::Event>();
        if (event.type == Person::Event::Type::FIRED)
        {
            mState = State::FAILED;
            return true;
        }
    }
    return false;
}

std::string GoalWork::toString() const
{
    return "Work";
}
