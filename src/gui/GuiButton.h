#pragma once

#include <string>
#include "gui/GuiWidget.h"
#include "gui/GuiStyle.h"

class GuiStyle;

class GuiButton : public GuiWidget
{
public:
    GuiButton(const GuiStyle& style);
    GuiButton(const GuiStyle& style, const std::string& text, sf::Vector2f dimensions,
        unsigned int characterSize, const std::string& message);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual sf::Vector2f getPosition() const override;
    virtual void setPosition(sf::Vector2f position) override;
    virtual sf::Vector2f getSize() const override;
    virtual void setSize(sf::Vector2f size) override;
    virtual sf::FloatRect getRect() const override;

    std::string getMessage() const;

    void setText(const std::string& text);
    void setHighlight(bool highlight);
    void setOrigin(sf::Vector2f origin);
    void resize(sf::Vector2f dimensions, unsigned int characterSize);

    bool hitButton(sf::Vector2f position) const;

private:
    // Reference?
    GuiStyle mStyle;
    // Handles appearance of the button
    sf::RectangleShape mShape;
    // Text displayed on the button
    sf::Text mText;
    // String returned when the button is activated
    std::string mMessage;
};
