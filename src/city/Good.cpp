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
