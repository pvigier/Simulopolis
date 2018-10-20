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

// My includes
#include "util/IdManager.h"
#include "message/Mailbox.h"

/**
 * \brief Container for mailboxes that allows them to communicate easily
 *
 * \author Pierre Vigier
 */
class MessageBus
{
public:
    /**
     * \brief Default constructor
     */
    MessageBus();

    /**
     * \brief Send a message
     *
     * The receiver of the message must be valid. However the sender can be
     * UNDEFINED.
     *
     * Note that the message is pass by value.
     *
     * \param message Message to send
     */
    void send(Message message);

    /**
     * \brief Add a mailbox
     *
     * The mailbox's id is updated.
     *
     * \param mailbox Mailbox to add
     */
    void addMailbox(Mailbox& mailbox);

    /**
     * \brief Remove a mailbox
     *
     * The mailbox's id is set to UNDEFINED.
     *
     * \param mailbox Mailbox to remove
     */
    void removeMailbox(Mailbox& mailbox);

private:
    IdManager<Mailbox*> mMailboxes; /**< IdManager that manages the mailboxes */

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mMailboxes;
    }
};
