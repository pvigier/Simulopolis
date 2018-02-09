#pragma once

#include <vector>
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>

class GuiStyle
{
public:
    sf::Color bodyColor;
    sf::Color bodyHighlightColor;
    sf::Color borderColor;
    sf::Color borderHighlightColor;
    float borderSize;
    sf::Color textColor;
    sf::Color textHighlightColor;
    sf::Font* font;

    GuiStyle();
    GuiStyle(sf::Color bodyColor, sf::Color bodyHighlightColor,
        sf::Color borderColor, sf::Color borderHighlightColor, float borderSize,
        sf::Color textColor, sf::Color textHighlightColor, sf::Font* font);
};
