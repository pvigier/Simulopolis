#pragma once

#include <SFML/Graphics.hpp>
#include "message/Subject.h"

class GuiWidget : public sf::Drawable, public Subject
{
public:
    GuiWidget();
    virtual ~GuiWidget();

    virtual void update();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final override;

    // Parameters
    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(sf::Vector2f position) = 0;
    virtual sf::Vector2f getSize() const = 0;
    virtual void setSize(sf::Vector2f size) = 0;
    virtual sf::FloatRect getRect() const = 0;
    bool isVisible() const;
    void setVisible(bool visible);

    // Events
    void updateMouseMoved(sf::Vector2f position);
    void updateMouseButtonPressed(sf::Vector2f position);
    void updateMouseButtonReleased(sf::Vector2f position);

protected:
    bool mVisible;

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const = 0;

    // Events
    virtual void onHover(sf::Vector2f position);
    virtual void onPress(sf::Vector2f position);
    virtual void onRelease(sf::Vector2f position);
};
