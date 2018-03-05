#pragma once

#include <string>
#include "gui/GuiWidget.h"
#include "gui/GuiStyle.h"

class GuiStyle;
class PropertyList;

class GuiButton : public GuiWidget
{
public:
    GuiButton(sf::Vector2f size, Message message, const GuiStyle& style);
    GuiButton(const PropertyList& properties);

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setSize(sf::Vector2f size) override;

    Message getMessage() const;

    void setHighlight(bool highlight);

    bool hitButton(sf::Vector2f position) const;

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void onHover(sf::Vector2f position) override;
    virtual void onPress(sf::Vector2f position) override;

private:
    // Reference?
    GuiStyle mStyle;
    // Handles appearance of the button
    sf::RectangleShape mShape;
    // Integer returned when the button is activated
    Message mMessage;
};
