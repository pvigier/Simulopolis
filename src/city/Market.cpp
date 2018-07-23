#include "city/Market.h"

MessageBus* VMarket::sMessageBus = nullptr;

VMarket::VMarket(Type type) : mTime(0), mType(type)
{

}

void VMarket::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}
