#include "message/Subject.h"
#include "message/MessageBus.h"

Subject::Subject() : mSubjectMessageBus(nullptr)
{

}

Subject::~Subject()
{

}

void Subject::setSubjectMessageBus(MessageBus* messageBus)
{
    mSubjectMessageBus = messageBus;
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
    for (const Id& subscriberId : mSubscribers)
    {
        message.receiver = subscriberId;
        mSubjectMessageBus->send(message);
    }
}
