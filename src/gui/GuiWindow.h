#pragma once

#include "gui/GuiWidget.h"

class XmlDocument;

class GuiWindow : public GuiWidget
{
public:
    GuiWindow(const std::string& title, const XmlDocument* style);
    GuiWindow(const PropertyList& properties);

    virtual void setPosition(sf::Vector2f position) override;
    void setTitle(const std::string& title);

    virtual bool hasGuiEvents() const override;

protected:
    virtual void setSize(sf::Vector2f size) override;

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual bool onHover(sf::Vector2f position, bool processed) override;
    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onRelease(sf::Vector2f position, bool processed) override;

    virtual void applyStyle();

private:
    // Handles appearance of the window
    sf::RectangleShape mBar;
    sf::Text mTitle;
    sf::CircleShape mCloseButton;
    // Events
    bool mOnMove;
    sf::Vector2f mAnchor;
};
