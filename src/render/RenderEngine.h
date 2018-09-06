#pragma once

// SFML
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
     * \brief Return the size of the viewport
     *
     * \return Size of the viewport
     */
    sf::Vector2i getViewportSize() const;

    /**
     * \brief Return the offset of the viewport
     *
     * \return Offset of the viewport
     */
    sf::Vector2i getViewportOffset() const;

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
     * \brief Convert a point from target coordinates to world coordinates
     *
     * \param point Pixel to convert
     * \param view	The view to use for converting the point
     *
     * \return The converted point, in "world" units
     */
    sf::Vector2f mapPixelToCoords(const sf::Vector2i &point, const sf::View &view) const;

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
    sf::RenderTexture mRenderTexture; /**< Texture that is drawn on the window */
    sf::Vector2i mViewportOffset; /** Offset of the viewport */
    sf::Sprite mSprite; /**< Sprite used to draw the render texture on the window */
};
