#include "ai/Goal.h"

Goal::Goal()
{

}

Goal::~Goal()
{

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

void Goal::pushFront(Goal* goal)
{
    mSubgoals.emplace_front(goal);
}

void Goal::pushBack(Goal* goal)
{
    mSubgoals.emplace_back(goal);
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
        mSubgoals.pop_front();
    return mSubgoals.front()->getState();
}
