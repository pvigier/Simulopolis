#include "gui/GuiStyle.h"

GuiStyle::GuiStyle()
{

}

GuiStyle::GuiStyle(sf::Color bodyColor, sf::Color bodyHighlightColor,
    sf::Color borderColor, sf::Color borderHighlightColor, float borderSize,
    sf::Color textColor, sf::Color textHighlightColor, const sf::Font* font) :
    bodyColor(bodyColor), bodyHighlightColor(bodyHighlightColor),
    borderColor(borderColor), borderHighlightColor(borderHighlightColor), borderSize(borderSize),
    textColor(textColor), textHighlightColor(textHighlightColor), font(font)
{

}
