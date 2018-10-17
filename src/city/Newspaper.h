#pragma once

#include <string>
#include <vector>
#include <boost/serialization/access.hpp>

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

class Newspaper
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

    const std::string& getName() const;
    void setName(std::string name);
    const Edition& getLastEdition() const;

private:
    std::string mName;
    std::vector<Edition> mEditions;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mName & mEditions;
    }
};
