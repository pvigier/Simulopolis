#include "gui/GuiEntry.h"

GuiEntry::GuiEntry(const GuiStyle& style) : mStyle(style)
{

}

GuiEntry::GuiEntry(const GuiStyle& style, const std::string& text, sf::Vector2f dimensions,
    unsigned int characterSize, const std::string& message) :
    mStyle(style), mShape(dimensions), mText(text, *mStyle.font, characterSize), mMessage(message)
{
    mShape.setOutlineThickness(-mStyle.borderSize);
    setHighlight(false);
}

void GuiEntry::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
    target.draw(mText);
}

std::string GuiEntry::getMessage() const
{
    return mMessage;
}

void GuiEntry::setText(const std::string& text)
{
    mText.setString(text);
}

void GuiEntry::setHighlight(bool highlight)
{
    if (highlight)
    {
        mShape.setFillColor(mStyle.bodyHighlightColor);
        mShape.setOutlineColor(mStyle.borderHighlightColor);
        mText.setFillColor(mStyle.textHighlightColor);
    }
    else
    {
        mShape.setFillColor(mStyle.bodyColor);
        mShape.setOutlineColor(mStyle.borderColor);
        mText.setFillColor(mStyle.textColor);
    }
}

void GuiEntry::setOrigin(sf::Vector2f origin)
{
    mShape.setOrigin(origin);
    mText.setOrigin(origin);
}

void GuiEntry::setPosition(sf::Vector2f position)
{
    mShape.setPosition(position);
    mText.setPosition(position);
}

void GuiEntry::resize(sf::Vector2f dimensions, unsigned int characterSize)
{
    mShape.setSize(dimensions);
    mText.setCharacterSize(characterSize);
}

bool GuiEntry::hitButton(sf::Vector2f position) const
{
    return mShape.getGlobalBounds().contains(position);
}
