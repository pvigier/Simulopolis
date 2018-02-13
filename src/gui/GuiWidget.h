#pragma once

#include <SFML/Graphics.hpp>
#include "message/Subject.h"

class GuiWidget : public sf::Drawable, public Subject
{
public:
    GuiWidget();
    virtual ~GuiWidget();

    virtual void update();

    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(sf::Vector2f position) = 0;
    virtual sf::Vector2f getSize() const = 0;
    virtual void setSize(sf::Vector2f size) = 0;
    virtual sf::FloatRect getRect() const = 0;

protected:

};
