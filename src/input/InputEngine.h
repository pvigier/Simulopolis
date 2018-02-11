#pragma once

// SFML
#include <SFML/Window.hpp>
// My includes
#include "message/Subject.h"

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
     * \param window Window created by the render engine
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
