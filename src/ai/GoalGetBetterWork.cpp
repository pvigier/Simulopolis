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

#include "ai/GoalGetBetterWork.h"
#include "city/City.h"
#include "city/Person.h"
#include "city/Market.h"
#include "city/Lease.h"
#include "city/Housing.h"
#include "city/Work.h"

GoalGetBetterWork::GoalGetBetterWork(Person* owner, unsigned int nbMonthsBeforeFailing) :
    Goal(owner), mNbMonthsBeforeFailing(nbMonthsBeforeFailing), mMarket(nullptr)
{
    //ctor
}

void GoalGetBetterWork::activate()
{
    mState = State::ACTIVE;
    // Choose the right market
    mMarket = static_cast<const Market<Work>*>(mOwner->getCity()->getMarket(MarketType::WORK));
}

Goal::State GoalGetBetterWork::process()
{
    activateIfInactive();

    // Try to obtain a better work
    if (!isCompleted())
    {
        Qualification qualification = mOwner->getQualification();
        const Work* work = mOwner->getWork();
        for (const Market<Work>::Item* item : mMarket->getItems())
        {
            if (static_cast<int>(qualification) >= static_cast<int>(item->good->getQualification()) &&
                (!work || item->good->getSalary() > work->getSalary()) &&
                mOwner->getCity()->getMap().isReachableFrom(mOwner->getHome()->getHousing(), item->good->getWorkplace()))
                mOwner->getMessageBus()->send(Message::create(mOwner->getMailboxId(), mMarket->getMailboxId(), MessageType::MARKET, mMarket->createBidEvent(item->id, item->reservePrice)));
        }
        if (mNbMonthsBeforeFailing == 0)
            mState = State::FAILED;
    }
    --mNbMonthsBeforeFailing;

    return mState;
}

void GoalGetBetterWork::terminate()
{

}

bool GoalGetBetterWork::handle(Message message)
{
    if (message.type == MessageType::MARKET)
    {
        const Market<Work>::Event& event = message.getInfo<Market<Work>::Event>();
        if (event.type == Market<Work>::Event::Type::PURCHASE)
        {
            mOwner->quitWork();
            mOwner->setWork(event.sale.good);
            mState = State::COMPLETED;
            return true;
        }
    }
    return false;
}

std::string GoalGetBetterWork::toString() const
{
    return "Get a better work";
}
