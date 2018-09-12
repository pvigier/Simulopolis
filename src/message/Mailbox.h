#pragma once

// STL
#include <deque>
// Boost
#include <boost/serialization/deque.hpp>
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
     * \brief Desctructor
     */
    ~Mailbox();

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

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mId & mMessages;
    }
};
