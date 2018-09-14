#pragma once

#include <string>
#include <boost/serialization/access.hpp>

class Industry;

class Good
{
public:
    enum class Type : int {NECESSARY = 0, NORMAL, LUXURY};

    static std::string typeToString(Good::Type type);
    static float typeToHappiness(Good::Type type);

    Good(Type type, Industry* productionPlace);

    Type getType() const;
    const Industry* getProductionPlace() const;

private:
    Type mType;
    Industry* mProductionPlace;

    // Serialization
    friend class boost::serialization::access;

    Good() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mType & mProductionPlace;
    }
};

