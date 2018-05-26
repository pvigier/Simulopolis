#pragma once

#include <string>
#include "gui/GuiWidget.h"

class XmlDocument;

class GuiButton : public GuiWidget
{
public:
    enum class State{NORMAL, HOVERED, PRESSED, FORCE_PRESSED, DISABLED};

    GuiButton(sf::Vector2f size, Message message, const XmlDocument* style);
    GuiButton(const PropertyList& properties);

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setSize(sf::Vector2f size) override;

    void setState(State state);

    bool hitButton(sf::Vector2f position) const;

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual bool onHover(sf::Vector2f position) override;
    virtual bool onPress(sf::Vector2f position) override;
    virtual bool onRelease(sf::Vector2f position) override;

private:
    const XmlDocument* mStyle;
    State mState;
    // Handles appearance of the button
    sf::RectangleShape mShape;
};
