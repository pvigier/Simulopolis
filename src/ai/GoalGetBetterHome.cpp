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
 
#include "GoalGetBetterHome.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Market.h"
#include "city/Lease.h"
#include "city/Housing.h"

GoalGetBetterHome::GoalGetBetterHome(Person* owner, unsigned int nbMonthsBeforeFailing) :
    Goal(owner), mNbMonthsBeforeFailing(nbMonthsBeforeFailing), mMarket(nullptr)
{
    //ctor
}

GoalGetBetterHome::~GoalGetBetterHome()
{
    //dtor
}

void GoalGetBetterHome::activate()
{
    mState = State::ACTIVE;
    // Choose the right market
    mMarket = static_cast<const Market<Lease>*>(mOwner->getCity()->getMarket(VMarket::Type::RENT));
}

Goal::State GoalGetBetterHome::process()
{
    activateIfInactive();

    // Try to obtain a better home
    if (!isCompleted())
    {
        const Lease* home = mOwner->getHome();
        sf::Vector2i carCoords = mOwner->getCity()->toTileIndices(mOwner->getCar().getKinematic().getPosition());
        const Tile* tile = mOwner->getCity()->getMap().getTile(carCoords.y, carCoords.x);
        for (const Market<Lease>::Item* item : mMarket->getItems())
        {
             if ((!home ||
                 item->good->getHousing()->getComfort() > home->getHousing()->getComfort() ||
                 (item->good->getHousing()->getComfort() >= home->getHousing()->getComfort() && item->good->getRent() < home->getRent())) &&
                (mOwner->getCity()->getMap().isReachableFrom(tile, item->good->getHousing())))
                mOwner->getMessageBus()->send(Message::create(mOwner->getMailboxId(), mMarket->getMailboxId(), MessageType::MARKET, mMarket->createBidEvent(item->id, item->reservePrice)));
        }
    }
    else if (mNbMonthsBeforeFailing == 0)
        mState = State::FAILED;
    --mNbMonthsBeforeFailing;

    return mState;
}

void GoalGetBetterHome::terminate()
{

}

bool GoalGetBetterHome::handle(Message message)
{
    if (message.type == MessageType::MARKET)
    {
        const Market<Lease>::Event& event = message.getInfo<Market<Lease>::Event>();
        if (event.type == Market<Lease>::Event::Type::PURCHASE)
        {
            mOwner->leaveHome();
            mOwner->setHome(event.sale.good);
            mState = State::COMPLETED;
            return true;
        }
    }
    return false;
}

std::string GoalGetBetterHome::toString() const
{
    return "Get a better home";
}
