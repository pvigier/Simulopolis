#pragma once

// STL
#include <vector>
#include <algorithm>
// Boost
#include <boost/serialization/vector.hpp>
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
    ~Subject();

    /**
     * \brief Set the message bus
     */
    static void setMessageBus(MessageBus* messageBus);

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
    static MessageBus* sMessageBus; /**< Message bus */
    std::vector<Id> mSubscribers; /**< Ids of the subscribed mailboxes */

    // Serialization
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mSubscribers;
    }
};
