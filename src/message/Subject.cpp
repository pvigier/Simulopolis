#include "message/Subject.h"
#include "message/MessageBus.h"

MessageBus* Subject::sMessageBus = nullptr;

Subject::Subject()
{

}

Subject::~Subject()
{

}

void Subject::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

void Subject::subscribe(Id id)
{
    mSubscribers.push_back(id);
}

void Subject::unsubscribe(Id id)
{
    std::vector<Id>::iterator it = std::find(mSubscribers.begin(), mSubscribers.end(), id);
    mSubscribers.erase(it);
}

void Subject::notify(Message message)
{
    for (std::size_t i = 0; i < mSubscribers.size(); ++i)
    {
        message.receiver = mSubscribers[i];
        sMessageBus->send(message);
    }
}
