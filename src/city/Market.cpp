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
 
#include "city/Market.h"

VMarket::EventBase::EventBase()
{

}

VMarket::EventBase::EventBase(VMarket::Type marketType) : marketType(marketType)
{

}

VMarket::EventBase::~EventBase()
{

}

VMarket::VMarket(Type type) : mMessageBus(nullptr), mTime(0), mType(type)
{

}

VMarket::~VMarket()
{
    if (mMailbox.getId() != UNDEFINED)
        mMessageBus->removeMailbox(mMailbox);
}

void VMarket::setMessageBus(MessageBus* messageBus, bool alreadyAdded)
{
    mMessageBus = messageBus;
    if (!alreadyAdded)
        mMessageBus->addMailbox(mMailbox);
}

Id VMarket::getMailboxId() const
{
    return mMailbox.getId();
}
