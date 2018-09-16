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
     * \brief Destructor
     */
    ~MessageBus();

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
