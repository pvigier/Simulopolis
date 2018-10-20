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
#include <deque>
// Boost
#include <boost/serialization/access.hpp>
// Message
#include "message/Message.h"

/**
 * \brief Container specialized in receiving messages
 *
 * Mailbox is a FIFO data structure.
 *
 * \see Message
 *
 * \author Pierre Vigier
 */
class Mailbox
{
public:
    /**
     * \brief Default constructor
     */
    Mailbox();

    /**
     * \brief Put a message
     *
     * \param message Message to put in the mailbox
     */
    void put(Message message);

    /**
     * \brief Get a message
     *
     * \return The oldest message in the container
     */
    Message get();

    /**
     * \brief Tell whether or not the mailbox is empty
     *
     * \return True if the mailbox is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * \brief Get the number of messages
     *
     * \return The number of messages in the mailbox
     */
    int getNbMessages() const;

    /**
     * \brief Get the id
     *
     * \return Id of the mailbox in the message bus it is registered in
     */
    Id getId() const;

    /**
     * \brief Set the id
     *
     * \param id Id of the mailbox in the message bus it is registered in
     */
    void setId(Id id);

private:
    Id mId; /**< Id */
    std::deque<Message> mMessages; /**< Queue of messages */

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mId & mMessages;
    }
};
