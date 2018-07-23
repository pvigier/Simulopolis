#include "GoalRest.h"
#include "city/Person.h"
#include "city/Housing.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalRest::GoalRest(Person* owner, float duration) : Goal(owner), mDuration(duration)
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
    pushBack(new GoalMoveTo(mOwner, mOwner->getHome()->getHousing()->getCoordinates()));
    pushBack(new GoalWait(mOwner, mDuration));
}

Goal::State GoalRest::process()
{
    activateIfInactive();

    mState = processSubgoals();
    return mState;
}

void GoalRest::terminate()
{
    mOwner->increaseHappiness(mOwner->getHome()->getHousing()->getComfort());
}
