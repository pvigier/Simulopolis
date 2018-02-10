#include "message/Mailbox.h"

Mailbox::Mailbox() : mId(-1)
{

}

Mailbox::~Mailbox()
{

}

void Mailbox::put(Message message)
{
    mMessages.push_back(message);
}

Message Mailbox::get()
{
    Message message(mMessages.front());
    mMessages.pop_front();
    return message;
}

bool Mailbox::isEmpty() const
{
    return mMessages.empty();
}

int Mailbox::getNbMessages() const
{
    return mMessages.size();
}

Id Mailbox::getId() const
{
    return mId;
}

void Mailbox::setId(Id id)
{
    mId = id;
}
