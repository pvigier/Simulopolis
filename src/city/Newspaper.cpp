#include "Newspaper.h"
#include "message/MessageBus.h"
#include "city/City.h"

Newspaper::Newspaper() : mCity(nullptr), mMessageBus(nullptr), mPersonGenerator(nullptr)
{
    mArticleGenerator.setUp();
}

Newspaper::~Newspaper()
{
    if (mMailbox.getId() != UNDEFINED)
        mMessageBus->removeMailbox(mMailbox);
}

void Newspaper::setCity(const City* city, MessageBus* messageBus, PersonGenerator* personGenerator, bool alreadyAdded)
{
    mCity = city;
    mMessageBus = messageBus;
    mPersonGenerator = personGenerator;
    if (!alreadyAdded)
    {
        mMessageBus->addMailbox(mMailbox);
        mMailbox.put(Message::create(MessageType::CITY, City::Event::Type::NEW_CITY));
        update();
    }
}

void Newspaper::update()
{
    Edition edition;
    edition.date = mCity->getPrettyDate();
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::CITY)
        {
            const City::Event& event = message.getInfo<City::Event>();
            switch (event.type)
            {
                case City::Event::Type::NEW_CITY:
                    edition.articles.emplace_back(mArticleGenerator.generate(ArticleGenerator::ArticleType::NEW_CITY,
                        ArticleGenerator::Context{{"city", mCity->getName()}}, mPersonGenerator->generateName()));
                    break;
                default:
                    break;
            }
        }

    }
    mEditions.emplace_back(std::move(edition));
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
