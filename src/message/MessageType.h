#pragma once

enum class MessageType
{
    INPUT,
    GUI,
    GUI_WINDOW,
    PUSH_GAME_STATE,
    POP_GAME_STATE,
    CHANGE_GAME_STATE,
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
        case MessageType::GUI_WINDOW:
            os << "gui window";
            break;
        case MessageType::PUSH_GAME_STATE:
            os << "push game state";
            break;
        case MessageType::POP_GAME_STATE:
            os << "pop game state";
            break;
        case MessageType::CHANGE_GAME_STATE:
            os << "change game state";
            break;
        default:
            os << "unknown";
            break;
    }
    return os;
}
