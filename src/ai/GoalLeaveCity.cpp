#include "ai/GoalLeaveCity.h"
#include "city/City.h"
#include "ai/GoalMoveTo.h"

#include <iostream>

GoalLeaveCity::GoalLeaveCity(Person* owner) : Goal(owner)
{
    //ctor
}

GoalLeaveCity::~GoalLeaveCity()
{
    //dtor
}

void GoalLeaveCity::activate()
{
    mState = State::ACTIVE;

    // Add subgoals
    clearSubgoals();
    // Leave home and work
    mOwner->leaveHome();
    mOwner->quitWork();
    // Look for an exit point
    sf::Vector2i exitPoint;
    sf::Vector2i carCoords = mOwner->getCity()->toTileIndices(mOwner->getCar().getKinematic().getPosition());
    if (mOwner->getCity()->getMap().getNetwork().getRandomEntryPoint(carCoords.y, carCoords.x, exitPoint))
    {
		const Tile* tile = mOwner->getCity()->getMap().getTile(exitPoint.y, exitPoint.x);
        pushBack(std::make_unique<GoalMoveTo>(mOwner, tile));
    }
}

Goal::State GoalLeaveCity::process()
{
    activateIfInactive();
    mState = processSubgoals();
    return mState;
}

void GoalLeaveCity::terminate()
{
    mOwner->getMessageBus()->send(Message::create(mOwner->getCity()->getMailboxId(), MessageType::CITY, City::Event(City::Event::Type::REMOVE_CITIZEN, mOwner)));
}

std::string GoalLeaveCity::toString() const
{
    return "Leave the city";
}

