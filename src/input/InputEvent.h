#pragma once

#include <SFML/Window/Event.hpp>

struct InputEvent : sf::Event
{
    bool processed;

    InputEvent(sf::Event event) : sf::Event(event), processed(false)
    {

    }
};
