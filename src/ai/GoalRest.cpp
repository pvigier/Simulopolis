#include "GoalRest.h"
#include "city/Person.h"
#include "city/Housing.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalRest::GoalRest(Person* owner) : Goal(owner)
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
    // Compute the number of hours needed to be rested
    float nbHours = (1.0f - mOwner->getSleep()) * 100.0f; // Temporary
    // Add subgoals
    clearSubgoals();
    pushBack(new GoalMoveTo(mOwner, mOwner->getHome()->getHousing()));
    pushBack(new GoalWait(mOwner, nbHours));
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

std::string GoalRest::toString() const
{
    return "Rest";
}
