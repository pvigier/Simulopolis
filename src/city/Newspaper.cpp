#include "Newspaper.h"
#include "message/MessageBus.h"

Newspaper::Newspaper()
Newspaper::Newspaper() : mMessageBus(nullptr)
{
    mArticleGenerator.setUp();

}

Newspaper::~Newspaper()
{
    if (mMailbox.getId() != UNDEFINED)
        mMessageBus->removeMailbox(mMailbox);
}

void Newspaper::setMessageBus(MessageBus* messageBus, bool alreadyAdded)
{
    mMessageBus = messageBus;
    if (!alreadyAdded)
        mMessageBus->addMailbox(mMailbox);
}

const std::string& Newspaper::getName() const
{
    return mName;
}

void Newspaper::setName(std::string name)
{
    mName = std::move(name);
}

const Newspaper::Edition& Newspaper::getLastEdition() const
{
    return mEditions.back();
}
