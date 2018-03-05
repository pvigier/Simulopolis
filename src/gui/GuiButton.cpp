#include "gui/GuiButton.h"
#include "resource/ResourceManager.h"

GuiButton::GuiButton(sf::Vector2f size, Message message, const GuiStyle& style) :
    mStyle(style), mMessage(message)
{
    setSize(size);
    mShape.setOutlineThickness(-mStyle.borderSize);
    setHighlight(false);
}

GuiButton::GuiButton(const PropertyList& properties) :
    GuiWidget(properties), mMessage(Message::create(MessageType::GUI))
{
    mMessage = Message::create(MessageType::GUI, properties.get<std::string>("message", ""));
    mStyle = properties.get<const GuiStyle&>("style");
    mShape.setPosition(mPosition);
    mShape.setSize(mSize);
    mShape.setOutlineThickness(-mStyle.borderSize);
    setHighlight(properties.get<bool>("highlight", false));
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
