#include "city/Market.h"

VMarket::EventBase::EventBase(VMarket::Type marketType) : marketType(marketType)
{

}

VMarket::VMarket(Type type) : mTime(0), mType(type)
{
    sMessageBus->addMailbox(mMailbox);
}

VMarket::~VMarket()
{

}

Id VMarket::getMailboxId() const
{
    return mMailbox.getId();
}
