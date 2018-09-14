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
