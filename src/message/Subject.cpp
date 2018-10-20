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

#include "message/Subject.h"
#include "message/MessageBus.h"

Subject::Subject() : mSubjectMessageBus(nullptr)
{

}

Subject::~Subject()
{

}

void Subject::setSubjectMessageBus(MessageBus* messageBus)
{
    mSubjectMessageBus = messageBus;
}

void Subject::subscribe(Id id)
{
    mSubscribers.push_back(id);
}

void Subject::unsubscribe(Id id)
{
    std::vector<Id>::iterator it = std::find(mSubscribers.begin(), mSubscribers.end(), id);
    mSubscribers.erase(it);
}

void Subject::notify(Message message)
{
    for (const Id& subscriberId : mSubscribers)
    {
        message.receiver = subscriberId;
        mSubjectMessageBus->send(message);
    }
}
