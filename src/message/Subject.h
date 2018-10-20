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

#pragma once

// STL
#include <vector>
#include <algorithm>
// My includes
#include "message/Message.h"

class MessageBus;

/**
 * \brief Implementation of the Observer design pattern for the messaging system
 *
 * \author Pierre Vigier
 */
class Subject
{
public:
    /**
     * \brief Default constructor
     */
    Subject();

    /**
     * \brief Destructor
     */
    virtual ~Subject();

    /**
     * \brief Set the message bus
     */
    void setSubjectMessageBus(MessageBus* messageBus);

    /**
     * \brief Subscribe a mailbox
     *
     * \param id Id of the mailbox to subscribe
     */
    void subscribe(Id id);

    /**
     * \brief Unsubscribe a mailbox
     *
     * \param id Id of the mailbox to unsubscribe
     */
    void unsubscribe(Id id);

    /**
     * \brief Send a message to every subscribed mailbox
     *
     * \param message Message to send
     */
    void notify(Message message);

protected:
    MessageBus* mSubjectMessageBus; /**< Message bus */
    std::vector<Id> mSubscribers; /**< Ids of the subscribed mailboxes */
};
