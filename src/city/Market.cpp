#include "city/Market.h"

MessageBus* VMarket::sMessageBus = nullptr;

void VMarket::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

VMarket::VMarket(Type type) : mTime(0), mType(type)
{

}
