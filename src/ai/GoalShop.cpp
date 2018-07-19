#include "GoalShop.h"
#include "city/Person.h"
#include "city/Business.h"
#include "city/City.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalShop::GoalShop(Person* owner) : Goal(owner)
{
    //ctor
}

GoalShop::~GoalShop()
{
    //dtor
}

void GoalShop::activate()
{
    mState = State::ACTIVE;
    // Add subgoals
    clearSubgoals();
    pushBack(new GoalMoveTo(mOwner, mOwner->getFavoriteShop()->getCoordinates()));
    pushBack(new GoalWait(mOwner, mOwner->getCity()->toHumanTime(4.0f * 1.0f)));
}

Goal::State GoalShop::process()
{
    mState = processSubgoals();
    return mState;
}

void GoalShop::terminate()
{

}
