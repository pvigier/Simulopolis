#pragma once

#include "gui/GuiWidget.h"

class XmlDocument;

class GuiWindow : public GuiWidget
{
public:
    GuiWindow(sf::Vector2f size, const XmlDocument* style);
    GuiWindow(const PropertyList& properties);

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setSize(sf::Vector2f size) override;

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void onHover(sf::Vector2f position) override;
    virtual void onPress(sf::Vector2f position) override;

private:
    const XmlDocument* mStyle;
    // Handles appearance of the window
    sf::RectangleShape mBar;
    sf::RectangleShape mBody;
    sf::Text mTitle;
    sf::FloatRect mCrossRect;

    void applyStyle();
};
