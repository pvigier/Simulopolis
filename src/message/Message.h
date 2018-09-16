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
#include <memory>
// My includes
#include "util/Id.h"
#include "message/MessageType.h"

/**
 * \brief Class that represents a message
 *
 * Note that the memory of the extra info is managed by a std::shared_ptr<void>.
 * If you have an unexpected behavior, check how the memory is managed.
 *
 * \see Id.h, MessageType.h
 *
 * \author Pierre Vigier
 */
class Message
{
public:
    Id sender; /**< Sender's id */
    Id receiver; /**< Receiver's id */
    MessageType type; /**< Type of the message */
    std::shared_ptr<void> info; /**< Extra info */

    /**
     * \brief Constructor for message without extra info
     *
     * \param sender Id of the sender
     * \param receiver Id of the receiver
     * \param type Type of the message
     */
    Message(Id sender = UNDEFINED, Id receiver = UNDEFINED, MessageType type = MessageType::UNKNOWN) :
        sender(sender), receiver(receiver), type(type), info()
    {

    }

    /**
     * \brief Constructor for message with extra info
     *
     * \param sender Id of the sender
     * \param receiver Id of the receiver
     * \param type Type of the message
     * \param info Shared pointer on the extra info
     */
    Message(Id sender, Id receiver, MessageType type, std::shared_ptr<void> info) :
        sender(sender), receiver(receiver), type(type), info(info)
    {

    }

    /**
     * \brief Constructor for message with undefined sender without extra info
     *
     * \param receiver Id of the receiver
     * \param type Type of the message
     */
    Message(Id receiver, MessageType type) :
        sender(UNDEFINED), receiver(receiver), type(type), info()
    {

    }

    /**
     * \brief Constructor for message with undefined sender and extra info
     *
     * \param receiver Id of the receiver
     * \param type Type of the message
     * \param info Shared pointer on the extra info
     */
    Message(Id receiver, MessageType type, std::shared_ptr<void> info) :
        sender(UNDEFINED), receiver(receiver), type(type), info(info)
    {

    }

    /**
     * \brief Constructor for message with undefined sender and receiver without extra info
     *
     * \param type Type of the message
     */
    Message(MessageType type) :
        sender(UNDEFINED), receiver(UNDEFINED), type(type), info()
    {

    }

    /**
     * \brief Constructor for message with undefined sender and receiver and extra info
     *
     * \param type Type of the message
     * \param info Shared pointer on the extra info
     */
    Message(MessageType type, std::shared_ptr<void> info) :
        sender(UNDEFINED), receiver(UNDEFINED), type(type), info(info)
    {

    }

    /**
     * \brief Create a message without extra info
     *
     * \param sender Id of the sender
     * \param receiver Id of the receiver
     * \param type Type of the message
     */
    static Message create(Id sender, Id receiver, MessageType type)
    {
        return Message(sender, receiver, type);
    }

    /**
     * \brief Create a message with extra info
     *
     * \param sender Id of the sender
     * \param receiver Id of the receiver
     * \param type Type of the message
     * \param info Const reference to the extra info
     */
    template<typename T>
    static Message create(Id sender, Id receiver, MessageType type, const T& info)
    {
        return Message(sender, receiver, type, std::make_shared<T>(info));
    }

    /**
     * \brief Create a message with undefined sender without extra info
     *
     * \param receiver Id of the receiver
     * \param type Type of the message
     */
    static Message create(Id receiver, MessageType type)
    {
        return Message(receiver, type);
    }

    /**
     * \brief Create a message with undefined sender and extra info
     *
     * \param receiver Id of the receiver
     * \param type Type of the message
     * \param info Const reference to the extra info
     */
    template<typename T>
    static Message create(Id receiver, MessageType type, const T& info)
    {
        return Message(receiver, type, std::make_shared<T>(info));
    }

    /**
     * \brief Create a message with undefined sender and receiver without extra info
     *
     * \param type Type of the message
     */
    static Message create(MessageType type)
    {
        return Message(type);
    }

    /**
     * \brief Create a message with undefined sender and receiver and extra info
     *
     * \param type Type of the message
     * \param info Const reference to the extra info
     */
    template<typename T>
    static Message create(MessageType type, const T& info)
    {
        return Message(type, std::make_shared<T>(info));
    }

    /**
     * \brief Return true if the message has extra info and false otherwise
     */
    bool hasInfo()
    {
        if (info)
            return true;
        return false;
    }


    /**
     * \brief Cast the extra info to the specified type
     */
    template<typename T>
    inline T& getInfo()
    {
        return *std::static_pointer_cast<T>(info);
    }

    /**
     * \brief Cast the extra info to the specified type
     */
    template<typename T>
    inline const T& getInfo() const
    {
        return *std::static_pointer_cast<T>(info);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Message& message)
{
    if (message.sender == UNDEFINED)
        os << "From UNDEFINED";
    else
    os << "From " << message.sender;
    if (message.receiver == UNDEFINED)
        os << " to UNDEFINED: " << message.type;
    else
        os << " to " << message.receiver << ": " << message.type;
    return os;
}

