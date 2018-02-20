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

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setSize(sf::Vector2f size) override;

    std::string getMessage() const;

    void setText(const std::string& text);
    void setHighlight(bool highlight);
    void resize(sf::Vector2f dimensions, unsigned int characterSize);

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
    // Text displayed on the button
    sf::Text mText;
    // String returned when the button is activated
    std::string mMessage;
};
