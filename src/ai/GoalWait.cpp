#include "GoalWait.h"

GoalWait::GoalWait(Person* owner, float duration) : Goal(owner), mDuration(duration)
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
    mClock.restart();
}

Goal::State GoalWait::process()
{
    activateIfInactive();

    if (mClock.getElapsedTime().asSeconds() >= mDuration)
        mState = State::COMPLETED;
}

void GoalWait::terminate()
{

}
