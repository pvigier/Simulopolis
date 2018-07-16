#include "ai/Goal.h"

Goal::Goal(Person* owner) : mOwner(owner)
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

bool Goal::forward(Message message)
{
    if (!mSubgoals.empty())
        return mSubgoals.front()->handle(message);
    return false;
}
