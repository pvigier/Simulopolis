#include "gui/GuiButton.h"
#include "resource/XmlDocument.h"

GuiButton::GuiButton(sf::Vector2f size, Message message, const XmlDocument* style) :
    mStyle(style), mMessage(message)
{
    setSize(size);
    mShape.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    setHighlight(false);
}

GuiButton::GuiButton(const PropertyList& properties) :
    GuiWidget(properties), mMessage(Message::create(MessageType::GUI))
{
    mMessage = Message::create(MessageType::GUI, properties.get<std::string>("message", ""));
    mStyle = properties.get<const XmlDocument*>("style");
    mShape.setPosition(mPosition);
    mShape.setSize(mSize);
    mShape.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
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

void GuiButton::onRelease(sf::Vector2f position)
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
        mShape.setFillColor(mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("highlightColor"));
        mShape.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("highlightColor"));
    }
    else
    {
        mShape.setFillColor(mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("color"));
        mShape.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color"));
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
