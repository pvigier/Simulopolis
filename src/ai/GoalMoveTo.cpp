#include "GoalMoveTo.h"
#include "city/City.h"
#include "city/Person.h"

GoalMoveTo::GoalMoveTo(Person* owner, const sf::Vector2i& target) : Goal(owner), mTarget(target)
{
    //ctor
}

GoalMoveTo::~GoalMoveTo()
{
    //dtor
}

void GoalMoveTo::activate()
{
    mState = State::ACTIVE;
    // Update the state of the Owner
    mOwner->setState(Person::State::MOVING);
    // Update the steering behavior
    sf::Vector2i position = mOwner->getCity()->toTileIndices(mOwner->getCar().getKinematic().getPosition());
    Path path = mOwner->getCity()->getMap().getPath(position, mTarget);
    if (path.isEmpty())
        mState = State::FAILED;
    else
        mOwner->getCar().getSteering().setPath(path);
}

Goal::State GoalMoveTo::process()
{
    activateIfInactive();

    if (mOwner->getCar().getSteering().getPath().isFinished())
        mState = State::COMPLETED;

    return mState;
}

void GoalMoveTo::terminate()
{

}
