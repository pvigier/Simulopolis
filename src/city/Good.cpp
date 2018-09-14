#include "city/Good.h"

std::string Good::typeToString(Good::Type type)
{
    switch (type)
    {
        case Good::Type::NECESSARY:
            return "Necessary";
        case Good::Type::NORMAL:
            return "Normal";
        case Good::Type::LUXURY:
            return "Luxury";
        default:
            return "";
    }
}

float Good::typeToHappiness(Good::Type type)
{
    switch (type)
    {
        case Good::Type::NECESSARY:
            return 0.0f;
        case Good::Type::NORMAL:
            return 5.0f;
        case Good::Type::LUXURY:
            return 20.0f;
        default:
            return 0.0f;
    }
}

Good::Good(Type type, Industry* productionPlace) : mType(type), mProductionPlace(productionPlace)
{

}

Good::Type Good::getType() const
{
    return mType;
}

const Industry* Good::getProductionPlace() const
{
    return mProductionPlace;
}
