#include "city/Market.h"

VMarket::EventBase::EventBase()
{

}

VMarket::EventBase::EventBase(VMarket::Type marketType) : marketType(marketType)
{

}

VMarket::EventBase::~EventBase()
{

}

VMarket::VMarket(Type type) : mMessageBus(nullptr), mTime(0), mType(type)
{

}

VMarket::~VMarket()
{
    if (mMailbox.getId() != UNDEFINED)
        mMessageBus->removeMailbox(mMailbox);
}

void VMarket::setMessageBus(MessageBus* messageBus, bool alreadyAdded)
{
    mMessageBus = messageBus;
    if (!alreadyAdded)
        mMessageBus->addMailbox(mMailbox);
}

Id VMarket::getMailboxId() const
{
    return mMailbox.getId();
}
