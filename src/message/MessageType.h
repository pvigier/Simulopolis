#pragma once

enum class MessageType
{
    INPUT,
    GUI,
    RESUME_GAME,
    NEW_GAME,
    LOAD_GAME,
    DISPLAY_MENU,
    MARKET,
    CITY,
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
        case MessageType::NEW_GAME:
            os << "new game";
            break;
        case MessageType::LOAD_GAME:
            os << "load game";
            break;
        case MessageType::MARKET:
            os << "market";
            break;
        case MessageType::CITY:
            os << "city";
            break;
        default:
            os << "unknown";
            break;
    }
    return os;
}
