#include "gui/GuiButton.h"

GuiButton::GuiButton(const GuiStyle& style) : mStyle(style)
{

}

GuiButton::GuiButton(const GuiStyle& style, const std::string& text, sf::Vector2f dimensions,
    unsigned int characterSize, const std::string& message) :
    mStyle(style), mShape(dimensions), mText(text, *mStyle.font, characterSize), mMessage(message)
{
    mShape.setOutlineThickness(-mStyle.borderSize);
    setHighlight(false);
}

void GuiButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
    target.draw(mText);
}

sf::Vector2f GuiButton::getPosition() const
{
    return mShape.getPosition();
}

void GuiButton::setPosition(sf::Vector2f position)
{
    mShape.setPosition(position);
    mText.setPosition(position);
}

sf::Vector2f GuiButton::getSize() const
{
    return mShape.getSize();
}

void GuiButton::setSize(sf::Vector2f size)
{
    mShape.setSize(size);
}

sf::FloatRect GuiButton::getRect() const
{
    return mShape.getGlobalBounds();
}

std::string GuiButton::getMessage() const
{
    return mMessage;
}

void GuiButton::setText(const std::string& text)
{
    mText.setString(text);
}

void GuiButton::setHighlight(bool highlight)
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

void GuiButton::setOrigin(sf::Vector2f origin)
{
    mShape.setOrigin(origin);
    mText.setOrigin(origin);
}

void GuiButton::resize(sf::Vector2f dimensions, unsigned int characterSize)
{
    mShape.setSize(dimensions);
    mText.setCharacterSize(characterSize);
}

bool GuiButton::hitButton(sf::Vector2f position) const
{
    return mShape.getGlobalBounds().contains(position);
}
