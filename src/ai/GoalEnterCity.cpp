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

#include "ai/GoalEnterCity.h"
#include "ai/GoalMoveTo.h"
#include "city/Person.h"
#include "city/City.h"
#include "city/Market.h"
#include "city/Housing.h"
#include "city/Lease.h"

GoalEnterCity::GoalEnterCity(Person* owner) : Goal(owner), mMarket(nullptr), mHomeFound(false)
{

}

GoalEnterCity::~GoalEnterCity()
{

}

void GoalEnterCity::activate()
{
    mState = State::ACTIVE;
    // Choose the right market
    mMarket = static_cast<const Market<Lease>*>(mOwner->getCity()->getMarket(MarketType::RENT));
}

Goal::State GoalEnterCity::process()
{
    activateIfInactive();

    if (!hasFailed())
    {
        // Try to obtain a home
        if (!mHomeFound)
        {
            for (const Market<Lease>::Item* item : mMarket->getItems())
                mOwner->getMessageBus()->send(Message::create(mOwner->getMailboxId(), mMarket->getMailboxId(), MessageType::MARKET, mMarket->createBidEvent(item->id, item->reservePrice)));
        }
        // Wait until the home is reachable
        else
        {
            sf::Vector2i entryPoint;
            sf::Vector2i housingCoords = mOwner->getHome()->getHousing()->getCoordinates();
            sf::Vector2i roadCoords;
            if (mOwner->getCity()->getMap().getNetwork().getAdjacentRoad(housingCoords.y, housingCoords.x, roadCoords) &&
                mOwner->getCity()->getMap().getNetwork().getRandomEntryPoint(roadCoords.y, roadCoords.x, entryPoint))
            {
                    // Set position
                    mOwner->getCar().getKinematic().setPosition(mOwner->getCity()->getMap().computePosition(entryPoint.y, entryPoint.x) + sf::Vector2f(Tile::HEIGHT, Tile::HEIGHT * 0.5f));
                    // Terminate
                    mState = State::COMPLETED;
            }
        }
    }

    return mState;
}

void GoalEnterCity::terminate()
{

}

bool GoalEnterCity::handle(Message message)
{
    if (message.type == MessageType::MARKET)
    {
        const Market<Lease>::Event& event = message.getInfo<Market<Lease>::Event>();
        if (event.type == Market<Lease>::Event::Type::PURCHASE)
        {
            mHomeFound = true;
            mOwner->setHome(event.sale.good);
            return true;
        }
    }
    else if (message.type == MessageType::PERSON)
    {
        const Person::Event& event = message.getInfo<Person::Event>();
        if (event.type == Person::Event::Type::EXPELLED)
        {
            mState = State::FAILED;
            return true;
        }
    }
    return false;
}

std::string GoalEnterCity::toString() const
{
    return "Enter the city";
}
