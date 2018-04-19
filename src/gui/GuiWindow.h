#pragma once

#include "gui/GuiWidget.h"

class XmlDocument;

class GuiWindow : public GuiWidget
{
public:
    struct Event
    {
        enum class Type{CLOSE};
        GuiWidget* window;
        Type type;
    };

    GuiWindow(sf::Vector2f size, const std::string& title, const XmlDocument* style);
    GuiWindow(const PropertyList& properties);

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setSize(sf::Vector2f size) override;
    void setTitle(const std::string& title);

    virtual bool hasGuiEvents() const override;

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void onHover(sf::Vector2f position) override;
    virtual void onPress(sf::Vector2f position) override;
    virtual void onRelease(sf::Vector2f position) override;

private:
    const XmlDocument* mStyle;
    // Handles appearance of the window
    sf::RectangleShape mBar;
    sf::RectangleShape mBody;
    sf::Text mTitle;
    sf::CircleShape mCloseButton;
    // Events
    bool mOnMove;
    sf::Vector2f mAnchor;

    void applyStyle();
};
