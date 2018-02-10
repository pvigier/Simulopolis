#include "message/MessageBus.h"

MessageBus::MessageBus()
{

}

MessageBus::~MessageBus()
{

}

void MessageBus::send(Message message)
{
    mMailboxes.get(message.receiver)->put(message);
}

void MessageBus::addMailbox(Mailbox& mailbox)
{
    Id id = mMailboxes.add(&mailbox);
    mailbox.setId(id);
}

void MessageBus::removeMailbox(Mailbox& mailbox)
{
    mMailboxes.erase(mailbox.getId());
    mailbox.setId(UNDEFINED);
}
