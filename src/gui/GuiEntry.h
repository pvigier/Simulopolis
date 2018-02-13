#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "message/Subject.h"
#include "gui/GuiStyle.h"

class GuiStyle;

class GuiEntry : public sf::Drawable, public Subject
{
public:
    GuiEntry(const GuiStyle& style);
    GuiEntry(const GuiStyle& style, const std::string& text, sf::Vector2f dimensions,
        unsigned int characterSize, const std::string& message);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::string getMessage() const;

    void setText(const std::string& text);
    void setHighlight(bool highlight);
    void setOrigin(sf::Vector2f origin);
    void setPosition(sf::Vector2f position);
    void resize(sf::Vector2f dimensions, unsigned int characterSize);

    bool hitButton(sf::Vector2f position) const;

private:
    // Reference?
    GuiStyle mStyle;
    // Handles appearance of the entry
    sf::RectangleShape mShape;
    // Text displayed on the entry
    sf::Text mText;
    // String returned when the entry is activated
    std::string mMessage;
};
