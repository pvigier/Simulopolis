#include "GoalMoveTo.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Building.h"

GoalMoveTo::GoalMoveTo(Person* owner, const Building* target) : Goal(owner), mTarget(target)
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
    sf::Vector2i start = mOwner->getCity()->toTileIndices(mOwner->getCar().getKinematic().getPosition());
    sf::Vector2i targetCoords = mTarget->getCoordinates();
    sf::Vector2i end;
    if (!mOwner->getCity()->getMap().getNetwork().getAdjacentRoad(targetCoords.y, targetCoords.x, end))
        mState = State::FAILED;
    else
    {
        Path path = mOwner->getCity()->getMap().getPath(start, end);
        if (path.isEmpty())
            mState = State::FAILED;
        else
            mOwner->getCar().getSteering().setPath(path);
    }
}

Goal::State GoalMoveTo::process()
{
    activateIfInactive();

    if (mState != State::FAILED && mOwner->getCar().getSteering().getPath().isFinished())
        mState = State::COMPLETED;

    return mState;
}

void GoalMoveTo::terminate()
{

}

std::string GoalMoveTo::toString() const
{
    return "Move to";
}
