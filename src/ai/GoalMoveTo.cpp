#include "GoalMoveTo.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Building.h"
#include "util/format.h"

GoalMoveTo::GoalMoveTo(Person* owner, const Tile* target) : Goal(owner), mTarget(target)
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

    Car& car = mOwner->getCar();
    if (mState != State::FAILED && car.getSteering().getPath().isFinished() &&
        car.getKinematic().getPosition().squaredDistanceTo(car.getSteering().getPath().getCurrentPoint()) < ARRIVE_DISTANCE)
        mState = State::COMPLETED;

    return mState;
}

void GoalMoveTo::terminate()
{

}

std::string GoalMoveTo::toString() const
{
    if (mTarget->isBuilding())
        return format("Move to building %d", static_cast<const Building*>(mTarget)->getId());
    else
        return "Move to the frontier";
}
