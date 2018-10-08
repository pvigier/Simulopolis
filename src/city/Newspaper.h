#pragma once

#include <string>
#include <boost/serialization/access.hpp>

class Newspaper
{
public:
    Newspaper();
    ~Newspaper();

    const std::string& getName() const;
    void setName(std::string name);

private:
    std::string mName;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mName;
    }
};
