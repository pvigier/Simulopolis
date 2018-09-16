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
 
#include "GoalShop.h"
#include "city/Person.h"
#include "city/Housing.h"
#include "city/Business.h"
#include "city/City.h"
#include "city/Lease.h"
#include "GoalMoveTo.h"
#include "GoalWait.h"

GoalShop::GoalShop(Person* owner) : Goal(owner), mSelectedShop(nullptr), mGoodReserved(false)
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

    // Select the right type of shop
    Tile::Type type = Business::getBusinessType(mOwner->getConsumptionHabit());

    // Find a shop
    std::vector<const Building*> buildings = mOwner->getCity()->getMap().getReachableBuildingsAround(mOwner->getHome()->getHousing(), RADIUS, type);
    for (const Building* building : buildings)
    {
        const Business* shop = static_cast<const Business*>(building);
        if (shop->hasPreparedGoods() && shop->getPrice() <= mOwner->getAccountBalance())
        {
            if (!mSelectedShop || shop->getPrice() < mSelectedShop->getPrice())
                mSelectedShop = shop;
        }
    }

    if (mSelectedShop)
    {
        // Reserve a good
        mOwner->getMessageBus()->send(Message::create(mOwner->getMailboxId(), mSelectedShop->getMailboxId(), MessageType::BUSINESS, Business::Event{Business::Event::Type::RESERVATION, {}, {}}));

        // Clear subgoals
        clearSubgoals();
    }
    else
        mState = State::FAILED;
}

Goal::State GoalShop::process()
{
    activateIfInactive();

    if (mGoodReserved)
        mState = processSubgoals();

    return mState;
}

void GoalShop::terminate()
{
    mOwner->increaseNeed(Person::Need::SATIETY, 1.0f);
    mOwner->increaseNeed(Person::Need::HAPPINESS, Good::typeToHappiness(mOwner->getConsumptionHabit()));
}

bool GoalShop::handle(Message message)
{
    if (message.type == MessageType::BUSINESS)
    {
        const Business::Event& event = message.getInfo<Business::Event>();
        if (event.type == Business::Event::Type::RESERVATION_ACCEPTED)
        {
            mGoodReserved = true;
            mOwner->getMessageBus()->send(Message::create(mOwner->getCity()->getBank().getMailboxId(), MessageType::BANK, mOwner->getCity()->getBank().createTransferMoneyEvent(mOwner->getAccount(), event.accountId, event.price)));
            pushBack(std::make_unique<GoalMoveTo>(mOwner, mSelectedShop));
            //pushBack(std::make_unique<GoalWait>(mOwner, mOwner->getCity()->toHumanTime(4.0f * 1.0f)));
            return true;
        }
        else if (event.type == Business::Event::Type::RESERVATION_REFUSED)
        {
            mState = State::FAILED;
            return true;
        }
    }
    return false;
}

std::string GoalShop::toString() const
{
    return "Shop";
}
