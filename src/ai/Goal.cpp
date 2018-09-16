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
 
#include "ai/Goal.h"

Goal::Goal(Person* owner) : mOwner(owner), mState(State::INACTIVE)
{

}

Goal::~Goal()
{

}

bool Goal::handle(Message message)
{
    return false;
}

Goal::State Goal::getState() const
{
    return mState;
}

bool Goal::isActive() const
{
    return mState == State::ACTIVE;
}

bool Goal::isInactive() const
{
    return mState == State::INACTIVE;
}

bool Goal::isCompleted() const
{
    return mState == State::COMPLETED;
}

bool Goal::hasFailed() const
{
    return mState == State::FAILED;
}

void Goal::pushFront(std::unique_ptr<Goal> goal)
{
    mSubgoals.emplace_front(std::move(goal));
}

void Goal::pushBack(std::unique_ptr<Goal> goal)
{
    mSubgoals.emplace_back(std::move(goal));
}

void Goal::clearSubgoals()
{
    mSubgoals.clear();
}

void  Goal::activateIfInactive()
{
    if (isInactive())
        activate();
}

void  Goal::reactivateIfFailed()
{
    if (hasFailed())
        mState = State::ACTIVE;
}

Goal::State Goal::processSubgoals()
{
    while (!mSubgoals.empty() && mSubgoals.front()->process() == State::COMPLETED)
    {
        mSubgoals.front()->terminate();
        mSubgoals.pop_front();
    }
    return mSubgoals.empty() ? State::COMPLETED : mSubgoals.front()->getState();
}

bool Goal::forward(Message message)
{
    if (!mSubgoals.empty())
        return mSubgoals.front()->handle(message);
    return false;
}

std::string Goal::toString() const
{
    return "";
}
