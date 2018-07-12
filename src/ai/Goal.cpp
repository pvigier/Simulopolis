#include "ai/Goal.h"

Goal::Goal()
{

}

Goal::~Goal()
{

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
