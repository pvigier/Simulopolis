#include "message/MessageBus.h"
#include "util/debug.h"

MessageBus::MessageBus()
{

}

MessageBus::~MessageBus()
{

}

void MessageBus::send(Message message)
{
    if (mMailboxes.has(message.receiver))
        mMailboxes.get(message.receiver)->put(message);
    else
        DEBUG(message << " can't be sent.\n");
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
