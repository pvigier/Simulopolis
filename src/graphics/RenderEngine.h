#pragma once

// SFML
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>

/**
 * \brief Engine that manages the window and the render scene
 *
 * \author Pierre Vigier
 */
class RenderEngine
{
public:
    /**
     * \brief Default constructor
     */
    RenderEngine();

    /**
     * \brief Destructor
     */
    ~RenderEngine();

    /**
     * \brief Get the window
     *
     * \return Window, an instance of the SFML's class RenderWindow
     */
    sf::RenderWindow& getWindow();

    /**
     * \brief Tell whether or not the window is open
     *
     * \return True if the window is open, false otherwise
     */
    bool isWindowOpen() const;

    /**
     * \brief Close the window
     */
    void closeWindow();

    /**
     * \brief Get the view
     *
     * \return View that is currently used, an instance of SFML's class View
     */
    const sf::View& getView() const;

    /**
     * \brief Set the view
     *
     * \param view New view view of the window
     */
    void setView(const sf::View& view);

    /**
     * \brief Clear the display surface
     */
    void clear();

    /**
     * \brief Draw a drawable
     */
    void draw(const sf::Drawable& drawable);

    /**
     * \brief Update the display surface
     */
     void display();

private:
    sf::RenderWindow mWindow; /**< Window that is opened */
};
