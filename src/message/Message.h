#pragma once

// STL
#include <iostream>
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
    Message(Id sender, Id receiver, MessageType type) :
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
     * \brief Cast the extra info to the specified type
     */
    template<typename T>
    inline T getInfo()
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

