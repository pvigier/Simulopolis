#include "message/MessageBus.h"

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
        std::cout << message << " can't be sent." << std::endl;
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
