#include "city/Good.h"

std::string goodToString(Good good)
{
    switch (good)
    {
        case Good::NECESSARY:
            return "Necessary";
        case Good::NORMAL:
            return "Normal";
        case Good::LUXURY:
            return "Luxury";
        default:
            return "";
    }
}

float goodToHappiness(Good good)
{
    switch (good)
    {
        case Good::NECESSARY:
            return 0.0f;
        case Good::NORMAL:
            return 5.0f;
        case Good::LUXURY:
            return 20.0f;
        default:
            return 0.0f;
    }
}
