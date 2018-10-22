#pragma once

#include <string>
#include <vector>
#include <boost/serialization/access.hpp>
#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/Mailbox.h"
#include "pcg/ArticleGenerator.h"

class MessageBus;

// Outside of Newspaper only for forward declaration in ArticleGenerator
struct Article
{
    std::string title;
    std::string body;
    std::string author;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & title & body & author;
    }
};

class Newspaper : public NonCopyable, public NonMovable
{
public:
    struct Edition
    {
        std::string date;
        std::vector<Article> articles;

        // Serialization
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & date & articles;
        }
    };

    Newspaper();
    ~Newspaper();

    void setMessageBus(MessageBus* messageBus, bool alreadyAdded = false);

    void update();

    const std::string& getName() const;
    void setName(std::string name);
    const Edition& getLastEdition() const;
    Id getMailboxId() const;

private:
    MessageBus* mMessageBus;
    ArticleGenerator mArticleGenerator;
    std::string mName;
    std::vector<Edition> mEditions;
    Mailbox mMailbox;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mName & mEditions & mMailbox;
    }
};
