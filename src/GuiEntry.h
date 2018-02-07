#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class GuiEntry
{
public:
    GuiEntry();
    GuiEntry(sf::RectangleShape shape, sf::Text text, const std::string& message);

    sf::RectangleShape& getShape();
    const sf::RectangleShape& getShape() const;
    sf::Text& getText();
    const sf::Text& getText() const;
    std::string getMessage() const;

private:
    // Handles appearance of the entry
    sf::RectangleShape mShape;
    // Text displayed on the entry
    sf::Text mText;
    // String returned when the entry is activated
    std::string mMessage;
};
