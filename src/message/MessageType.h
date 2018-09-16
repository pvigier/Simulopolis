/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

#include <iostream>

enum class MessageType
{
    UNKNOWN,
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
