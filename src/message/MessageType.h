#pragma once

#include <iostream>

enum class MessageType
{
    INPUT,
    GUI,
    GAME,
    MARKET,
    CITY,
    PERSON,
    BANK,
    BUSINESS,
};

inline std::ostream& operator<<(std::ostream& os, const MessageType& type)
{
    switch (type)
    {
        case MessageType::INPUT:
            os << "input";
            break;
        case MessageType::GUI:
            os << "gui";
            break;
        case MessageType::GAME:
            os << "game";
            break;
        case MessageType::MARKET:
            os << "market";
            break;
        case MessageType::CITY:
            os << "city";
            break;
        case MessageType::PERSON:
            os << "person";
            break;
        case MessageType::BANK:
            os << "bank";
            break;
        case MessageType::BUSINESS:
            os << "business";
            break;
        default:
            os << "unknown";
            break;
    }
    return os;
}
