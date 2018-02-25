#include "gui/GuiButton.h"

GuiButton::GuiButton(sf::Vector2f dimensions, Message message, const GuiStyle& style) :
    mStyle(style), mShape(dimensions), mMessage(message)
{
    mShape.setOutlineThickness(-mStyle.borderSize);
    setHighlight(false);
    GuiWidget::setSize(mShape.getSize());
}

void GuiButton::setPosition(sf::Vector2f position)
{
    GuiWidget::setPosition(position);
    mShape.setPosition(position);
}

void GuiButton::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    mShape.setSize(size);
}

void GuiButton::onHover(sf::Vector2f position)
{
    setHighlight(hitButton(position));
}

void GuiButton::onPress(sf::Vector2f position)
{
    if (hitButton(position))
        notify(mMessage);
}

Message GuiButton::getMessage() const
{
    return mMessage;
}

void GuiButton::setHighlight(bool highlight)
{
    if (highlight)
    {
        mShape.setFillColor(mStyle.bodyHighlightColor);
        mShape.setOutlineColor(mStyle.borderHighlightColor);
    }
    else
    {
        mShape.setFillColor(mStyle.bodyColor);
        mShape.setOutlineColor(mStyle.borderColor);
    }
}

bool GuiButton::hitButton(sf::Vector2f position) const
{
    return mShape.getGlobalBounds().contains(position);
}

void GuiButton::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
}
