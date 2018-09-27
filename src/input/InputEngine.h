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

#include <SFML/Window.hpp>
#include "message/Subject.h"
#include "input/InputEvent.h"

/**
 * \brief Engine that manages inputs
 *
 * Note that the InputEngine derived from Subject.
 *
 * \author Pierre Vigier
 */
class InputEngine : public Subject
{
public:
    /**
     * \brief Default constructor
     */
    InputEngine();

    /**
     * \brief Destructor
     */
    ~InputEngine();

    /**
     * \brief Set the window
     *
     * \param window Window from which the events are polled
     */
    void setWindow(sf::Window* window);

    /**
     * \brief Poll events
     *
     * This method send messages to subscribed mailboxes.
     */
    void pollEvents();

    /**
     * \brief Tell whether or not a key is pressed
     *
     * \param key SFML's code for a key
     *
     * \return True if the key is pressed, false otherwise
     */
    bool isKeyPressed(sf::Keyboard::Key key) const;

    /**
     * \brief Tell whether or not a mouse button is pressed
     *
     * \param button SFML's code for a mouse button
     *
     * \return True if the mouse button is pressed, false otherwise
     */
    bool isButtonPressed(sf::Mouse::Button button) const;

    /**
     * \brief Get the mouse position
     *
     * \return The mouse position relative to the origin of the window in pixels
     */
    sf::Vector2i getMousePosition() const;

private:
    sf::Window* mWindow; /**< Window from which the inputs are polled */
};
