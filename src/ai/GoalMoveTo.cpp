/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        {
            mOwner->getCar().getSteering().setPath(path);
            if (mOwner->getCar().getKinematic().getPosition().squaredDistanceTo(mOwner->getCar().getSteering().getPath().getLastPoint()) < MIN_DISTANCE)
                mState = State::COMPLETED;
        }
    }
}

Goal::State GoalMoveTo::process()
{
    activateIfInactive();

    Car& car = mOwner->getCar();
    if (mState != State::FAILED && car.getSteering().getPath().isFinished() &&
        car.getKinematic().getPosition().squaredDistanceTo(car.getSteering().getPath().getLastPoint()) < ARRIVE_DISTANCE)
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
